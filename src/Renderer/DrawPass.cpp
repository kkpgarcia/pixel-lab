#include "DrawPass.h"

DrawPass::DrawPass(OpenGLFramebuffer& gBuffer, OpenGLFramebuffer& drawBuffer) : _gBuffer(gBuffer), _drawBuffer(drawBuffer)
{
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &_quadVAO);
	glGenBuffers(1, &_quadVBO);
	glBindVertexArray(_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void DrawPass::Setup()
{
}

void DrawPass::Render(Scene& scene, Camera& camera)
{
	_drawBuffer.Bind();
	glBindVertexArray(_quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, _gBuffer.GetID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _drawBuffer.GetID());
	glBlitFramebuffer(0, 0, _drawBuffer.GetWidth(), _drawBuffer.GetHeight(), 0, 0, _drawBuffer.GetWidth(), _drawBuffer.GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_drawBuffer.Unbind();
}

void DrawPass::Cleanup()
{
}