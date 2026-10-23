# Per-Material Shaders

Reference notes on letting each material choose its own shader, and how the renderer switches between them.

## Overview

A material says **which shader** draws it and **what inputs** that shader gets: textures and parameters. The renderer groups draws by shader, then by material, and only switches when one changes.

```
Material  ──► Shader*          (nullptr = the renderer's default shader)
          ──► textures[]       (bound to units 0, 1, 2…)
          ──► parameters       (baseColor, uvScale, snowHeight…)

Renderer:  for each shader:
               use it, set per-frame uniforms (camera, light)
               for each material using that shader:
                   bind its textures, set its parameters
                   for each batch with that material:
                       draw instanced
```

Switching shaders is the most expensive state change, switching textures is cheaper, and switching uniforms is cheapest. Sorting the draws in that order keeps the expensive switches rare.

## Material

The material is general instead of hardcoding one albedo slot:

```cpp
struct TextureSlot {
    std::string      uniform;   // sampler name in the shader, e.g. "uGrass"
    const Texture2D* texture;
};

struct Material {
    const Shader*            shader = nullptr;   // nullptr → default shader
    std::vector<TextureSlot> textures;           // index = texture unit
    glm::vec4                baseColor{ 1.0f };
    glm::vec2                uvScale{ 1.0f };
    std::vector<std::pair<std::string, float>> floats;   // shader-specific values
};
```

Example: a terrain material set up in a scene.

```cpp
auto& assets = m_ctx.assetManager;
m_terrainMat.shader   = &assets.loadShader(ASSET_DIR "/shaders/default.vert", ASSET_DIR "/shaders/terrain.frag");
m_terrainMat.textures = {
    { "uGrass", &assets.loadTexture(ASSET_DIR "/textures/grass.jpg") },
    { "uRock",  &assets.loadTexture(ASSET_DIR "/textures/rock.jpg")  },
    { "uSnow",  &assets.loadTexture(ASSET_DIR "/textures/snow.jpg")  },
};
m_terrainMat.uvScale = glm::vec2(0.1f);
m_terrainMat.floats  = { { "uSnowHeight", 15.0f }, { "uBlend", 2.0f } };
```

- The terrain shader can **reuse `default.vert`** and only replace the fragment shader. The vertex shader's `out` variables must match the fragment shader's `in` variables.
- `AssetManager::loadShader` caches shaders by the pair of paths, the same way `loadTexture` caches by path.

## Renderer

```cpp
void Renderer::endFrame() {
    // draw order: sort batch indices by shader, then material
    m_order.clear();
    for (size_t i = 0; i < m_batches.size(); ++i)
        if (!m_batches[i].instances.empty()) m_order.push_back(i);

    std::sort(m_order.begin(), m_order.end(), [&](size_t a, size_t b) {
        const Batch& A = m_batches[a]; const Batch& B = m_batches[b];
        const Shader* sa = shaderFor(*A.material); const Shader* sb = shaderFor(*B.material);
        if (sa != sb) return sa < sb;
        return A.material < B.material;
    });

    const Shader*   boundShader = nullptr;
    const Material* boundMat    = nullptr;

    for (size_t idx : m_order) {
        Batch& batch = m_batches[idx];
        const Shader& shader = *shaderFor(*batch.material);

        if (&shader != boundShader) {
            shader.use();
            setFrameUniforms(shader);      // view, projection, light, camera position
            boundShader = &shader;
            boundMat = nullptr;            // a new shader needs its material uniforms set again
        }
        if (batch.material != boundMat) {
            applyMaterial(shader, *batch.material);
            boundMat = batch.material;
        }
        batch.model->draw(batch.instances);
        // ... stats ...
    }
}

const Shader* Renderer::shaderFor(const Material& m) const {
    return m.shader ? m.shader : &m_defaultShader;
}

void Renderer::applyMaterial(const Shader& s, const Material& m) {
    if (m.textures.empty()) {                       // simple materials: white texture in uAlbedo
        s.setInt("uAlbedo", 0);
        m_whiteTexture.bind(0);
    }
    for (int unit = 0; unit < (int)m.textures.size(); ++unit) {
        s.setInt(m.textures[unit].uniform, unit);
        (m.textures[unit].texture ? *m.textures[unit].texture : m_whiteTexture).bind(unit);
    }
    s.setVec4("uBaseColor", m.baseColor);
    s.setVec2("uUvScale",   m.uvScale);
    for (auto& [name, value] : m.floats) s.setFloat(name, value);
}
```

Notes:

- `m_order` is a `std::vector<size_t>` member. Sort indices rather than `m_batches` itself, because `m_batchIndex` stores positions into `m_batches`.
- The existing `Shader& m_shader` becomes `m_defaultShader`, used by any material without its own shader.
- **Uniforms belong to a shader program.** When you switch to another shader, it doesn't know the camera matrices you gave the previous one, so `setFrameUniforms` must run on every shader switch.

## Later: a uniform buffer for per-frame data

With a few shaders, re-sending the camera and light on each switch is fine. With many, use a **uniform buffer object (UBO)**: one GPU buffer that every shader reads, updated once per frame.

```glsl
// in every shader (requires #version 420 or higher for binding = 0)
layout(std140, binding = 0) uniform Frame {
    mat4 uView;
    mat4 uProjection;
    vec4 uLightDir;      // std140 pads vec3 to 16 bytes, so use vec4
    vec4 uLightColor;
    vec4 uViewPos;
};
```

```cpp
// C++: a matching struct, uploaded once in beginFrame
struct FrameData { glm::mat4 view, proj; glm::vec4 lightDir, lightColor, viewPos; };

// once at startup
glCreateBuffers(1, &m_frameUbo);
glNamedBufferData(m_frameUbo, sizeof(FrameData), nullptr, GL_DYNAMIC_DRAW);

// every frame
glNamedBufferSubData(m_frameUbo, 0, sizeof(FrameData), &data);
glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_frameUbo);
```

Then `setFrameUniforms` disappears, and every new shader automatically sees the camera. The shaders are currently `#version 330`; change them to `#version 460 core` to use `binding = 0` (the context is already 4.6).

## Things to watch

- **Shader-specific values in materials:** `floats` is a simple general solution. Some engines instead define a material type per shader (`TerrainMaterial`, `StandardMaterial`) with named fields. That's more type-safe but more code. The general version is fine at this stage.
- **Uniform lookups by name:** `glGetUniformLocation` runs a string lookup on every `set` call. Fine for now; if it shows up in profiling, cache the locations in `Shader` (`std::unordered_map<std::string, GLint>`).
- **Default material:** a material with no textures binds the 1×1 white texture to `uAlbedo`, so the default shader works unchanged for untextured objects.

## Implementation order

1. Add `shader` and `textures` to `Material`; keep `albedo` working until everything is migrated.
2. Add `AssetManager::loadShader(vert, frag)` with caching.
3. Rename `m_shader` to `m_defaultShader`; add `shaderFor`, `setFrameUniforms` and the shader switch in `endFrame`.
4. Add the sort by shader then material.
5. Write `terrain.frag` and give the terrain a terrain material.
6. Later: move per-frame uniforms into a UBO.
