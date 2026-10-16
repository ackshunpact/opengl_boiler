#include <engine/assets/ModelLoader.h>
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#include <stdexcept>
#include <memory>
#include <numeric>
#include <glm/gtc/type_ptr.hpp>

namespace {   // helpers private to this file

const char* cgltfResultToString(cgltf_result r) {
	switch (r) {
	case cgltf_result_success:         return "success";
	case cgltf_result_data_too_short:  return "data too short (truncated file?)";
	case cgltf_result_unknown_format:  return "unknown format (not glTF/GLB)";
	case cgltf_result_invalid_json:    return "invalid JSON";
	case cgltf_result_invalid_gltf:    return "invalid glTF structure";
	case cgltf_result_invalid_options: return "invalid options";
	case cgltf_result_file_not_found:  return "file not found";
	case cgltf_result_io_error:        return "I/O error";
	case cgltf_result_out_of_memory:   return "out of memory";
	case cgltf_result_legacy_gltf:     return "legacy glTF 1.0 (only 2.0 supported)";
	default:                           return "unknown error";
	}
}

void checkResult(cgltf_result result, const char* step, const std::string& path) {
	if (result != cgltf_result_success) {
		throw std::runtime_error(std::string(step) + " failed for '" + path + "': " + cgltfResultToString(result));
	}
}

const cgltf_accessor* findAttribute(const cgltf_primitive& p, cgltf_attribute_type type, int set = 0) {
	for (cgltf_size i = 0; i < p.attributes_count; ++i) {
		const cgltf_attribute& a = p.attributes[i];
		if (a.type == type && a.index == set) {
			return a.data;
		}
	}
	return nullptr;
}

engine::MeshData loadPrimitive(const cgltf_data* data, const cgltf_primitive& p, const glm::mat4& world) {
	engine::MeshData mesh;

	const cgltf_accessor* posAccessor = findAttribute(p, cgltf_attribute_type_position);
	const cgltf_accessor* normAccessor = findAttribute(p, cgltf_attribute_type_normal);
	const cgltf_accessor* uvAccessor = findAttribute(p, cgltf_attribute_type_texcoord);
	const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(world)));

	mesh.vertices.resize(posAccessor->count);
	for (cgltf_size i = 0; i < posAccessor->count; i++) {
		engine::Vertex& v = mesh.vertices[i];
		glm::vec3 pos;
		cgltf_accessor_read_float(posAccessor, i, glm::value_ptr(pos), 3);
		v.position = glm::vec3(world * glm::vec4(pos, 1.f));
		if (normAccessor) {
			glm::vec3 n;
			cgltf_accessor_read_float(normAccessor, i, glm::value_ptr(n), 3);
			v.normal = glm::normalize(normalMatrix * n);
		}
		if (uvAccessor) {
			cgltf_accessor_read_float(uvAccessor, i, glm::value_ptr(v.uv), 2);
		}
	}
	if (p.indices) {
		mesh.indices.resize(p.indices->count);
		for (cgltf_size i = 0; i < p.indices->count; ++i) {
			mesh.indices[i] = static_cast<uint32_t>(cgltf_accessor_read_index(p.indices, i));
		}
	}
	else {
		mesh.indices.resize(mesh.vertices.size());
		std::iota(mesh.indices.begin(), mesh.indices.end(), 0u);
	}

	mesh.materialIndex = p.material ? static_cast<int>(p.material - data->materials) : -1;
	return mesh;
}

void processNode(const cgltf_data* data, const cgltf_node* node, engine::ModelData& out) {
	if (node->mesh) {
		float m[16];
		cgltf_node_transform_world(node, m);
		const glm::mat4 world = glm::make_mat4(m);

		for (cgltf_size p = 0; p < node->mesh->primitives_count; ++p) {
			const cgltf_primitive& prim = node->mesh->primitives[p];
			if (prim.type != cgltf_primitive_type_triangles) continue;
			if (!findAttribute(prim, cgltf_attribute_type_position)) continue;

			out.meshes.push_back(loadPrimitive(data, prim, world));
		}
	}
	for (cgltf_size c = 0; c < node->children_count; ++c) {
		processNode(data, node->children[c], out);
	}
}

} // namespace

engine::ModelData engine::ModelLoader::loadGltf(const std::string& path)
{
	cgltf_options options = {};
	cgltf_data* raw = nullptr;
	checkResult(cgltf_parse_file(&options, path.c_str(), &raw), "cgltf_parse_file", path);

	std::unique_ptr<cgltf_data, decltype(&cgltf_free)> data(raw, &cgltf_free);
	checkResult(cgltf_load_buffers(&options, data.get(), path.c_str()), "cgltf_load_buffers", path);
	checkResult(cgltf_validate(data.get()), "cgltf_validate", path);

	ModelData model;
	const cgltf_scene* scene = data->scene ? data->scene : (data->scenes_count > 0 ? &data->scenes[0] : nullptr);
	if (scene) {
		for (cgltf_size i = 0; i < scene->nodes_count; i++) {
			processNode(data.get(), scene->nodes[i], model);
		}
	} else {
		for (cgltf_size i = 0; i < data->nodes_count; i++) {
			if (!data->nodes[i].parent) {
				processNode(data.get(), &data->nodes[i], model);
			}
		}
	}

    return model;
}
