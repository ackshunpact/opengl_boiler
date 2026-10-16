#include <engine/render/Renderer.h>

void engine::Renderer::beginFrame(const Camera& camera, const DirectionalLight& light)
{
	m_view = camera.viewMatrix();
	m_projection = camera.projectionMatrix();
	m_cameraPos = camera.getPosition();
	m_light = light;
	
	for (auto& batch : m_batches) {
		batch.instances.clear();
	}
	m_drawCalls = 0;

}

void engine::Renderer::submit(Model& model, const glm::mat4& transform)
{
	auto it = m_batchIndex.find(&model);
	if (it == m_batchIndex.end()) {
		it = m_batchIndex.emplace(&model, m_batches.size()).first;
		m_batches.push_back({ &model, {} });
	}
	m_batches[it->second].instances.push_back({ transform });
}

void engine::Renderer::endFrame()
{
	m_shader.use();
	m_shader.setMat4("uProjection", m_projection);
	m_shader.setMat4("uView", m_view);
	m_shader.setVec3("uLightDir", m_light.dir);
	m_shader.setVec3("uViewPos", m_cameraPos);
	m_shader.setVec3("uLightColor", m_light.color);

	for (auto& batch : m_batches) {
		if (batch.instances.empty()) continue;
		batch.model->draw(batch.instances);
		m_drawCalls += batch.model->meshCount();
	}
}
