#include <engine/render/Renderer.h>
#include <algorithm>

void engine::Renderer::beginFrame(const Camera& camera, const DirectionalLight& light)
{
	m_view = camera.viewMatrix();
	m_projection = camera.projectionMatrix();
	m_cameraPos = camera.getPosition();
	m_light = light;
	
	for (auto& batch : m_batches) {
		batch.instances.clear();
	}
	m_drawCalls = m_instanceCount = m_batchCount = 0;

}

void engine::Renderer::submit(Model& model, const Material* material, const glm::mat4& transform, const glm::vec4& color)
{
	if (!material) {
		material = &m_defaultMaterial;
	}
	BatchKey k{ &model, material };
	auto it = m_batchIndex.find(k);
	if (it == m_batchIndex.end()) {
		it = m_batchIndex.emplace(k, m_batches.size()).first;
		m_batches.push_back({ &model, material, {} });
	}
	m_batches[it->second].instances.push_back({ transform, color });
}

void engine::Renderer::endFrame()
{
	m_order.clear();
	for (size_t i = 0; i < m_batches.size(); ++i) {
		if (!m_batches[i].instances.empty()) {
			m_order.push_back(i);
		}
	}

	std::sort(m_order.begin(), m_order.end(), [&](size_t a, size_t b) {
		const Batch& A = m_batches[a];
		const Batch& B = m_batches[b];
		const Shader* sa = shaderFor(*A.material);
		const Shader* sb = shaderFor(*B.material);
		if (sa != sb) {
			return sa < sb;
		}
		return A.material < B.material;
		});



	const Shader* boundShader = nullptr;
	const Material* boundMat = nullptr;

	for (size_t idx : m_order) {
		Batch& batch = m_batches[idx];
		const Shader& shader = *shaderFor(*batch.material);
		if (&shader != boundShader) {
			shader.use();
			setFrameUniforms(shader);		// view, proj, light, camera
			boundShader = &shader;
			boundMat = nullptr;

		}
		if (batch.material != boundMat) {
			applyMaterial(shader, *batch.material);
			boundMat = batch.material;
		}

		batch.model->draw(batch.instances);
		m_drawCalls += batch.model->meshCount();
		m_instanceCount += batch.instances.size();
		++m_batchCount;
	
	}
}

void engine::Renderer::applyMaterial(const Shader& s, const Material& mat) {
	if (mat.textures.empty()) {
		s.setInt("uAlbedo", 0);
		m_whiteTexture.bind(0);
	}
	
	for (int unit = 0; unit < int(mat.textures.size()); ++unit) {
		s.setInt(mat.textures[unit].uniform, unit);
		(mat.textures[unit].texture ? *mat.textures[unit].texture : m_whiteTexture).bind(unit);
	}
	
	s.setVec4("uBaseColor", mat.baseColor);
	s.setVec2("uUvScale", mat.uvScale);
	for (auto& [name, value] : mat.floats) {
		s.setFloat(name, value);
	}
}

void engine::Renderer::setFrameUniforms(const Shader& s) const
{
	s.use();
	s.setMat4("uProjection", m_projection);
	s.setMat4("uView", m_view);
	s.setVec3("uLightDir", m_light.dir);
	s.setVec3("uViewPos", m_cameraPos);
	s.setVec3("uLightColor", m_light.color);
	s.setInt("uAlbedo", 0);
}


const engine::Shader* engine::Renderer::shaderFor(const Material& m) const {
	return m.shader ? m.shader : &m_defaultShader;
}

size_t engine::Renderer::lastInstanceCount() {
	return m_instanceCount;
}
size_t engine::Renderer::lastBatchCount() {
	return m_batchCount;
}
