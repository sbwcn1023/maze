/**
 * \brief
 * This class construct the MAZE.
 * Vertex list of a cube is stored in CVertices.
 * This class drawings cubes using "Instancing" method.
 */

#include <iostream>
#include <string.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CObject.h"
#include "CVertices.h"
#include "CShader.h"
#include "CBlock.h"
#include "CMovable.h"
#include "CModel.h"
#include "CPerspective.h"
#include "CView.h"
#include "CMisc.h"

using namespace std;

CBlock *CBlock::m_instance = NULL;

CBlock::CBlock(void)
: m_geometry_updated(true)
, m_color_updated(true)
, m_loaded(false)
{
	int i;

	int y;
	int x;

	char map[][7] = {
		{ 1, 1, 1, 1, 1, 1, 1, },
		{ 0, 0, 0, 0, 0, 0, 1, },
		{ 1, 0, 1, 0, 1, 0, 1, },
		{ 1, 1, 1, 0, 0, 0, 1, },
		{ 1, 0, 1, 1, 1, 0, 1, },
		{ 1, 0, 0, 0, 0, 0, 1, },
		{ 1, 1, 0, 1, 1, 1, 1, },
	};

	m_iCount = 0;
	for (y = 0; y < 7; y++)
		for (x = 0; x < 7; x++)
			m_iCount += (map[y][x] == 1);

	m_offset = (vec4 *)malloc(sizeof(*m_offset) * m_iCount);
	if (m_offset == NULL) {
		cerr << "Failed to allocate heap for map" << endl;
		return;
	}

	i = 0;
	for (y = 0; y < 7; y++) {
		for (x = 0; x < 7; x++) {
			if (map[y][x] == 0)
				continue;

			m_offset[i][0] = (x - 3) * (BLOCK_WIDTH * 2);
			m_offset[i][1] = 0.0f;
			m_offset[i][2] = (y - 3) * (BLOCK_WIDTH * 2);
			m_offset[i][3] = 1.0f;
			i++;
		}
	}

	cout << m_iCount << " instances are created " << i << endl;
	glGenBuffers(1, &m_VBO);
}

CBlock::~CBlock(void)
{
	free(m_offset);
	glDeleteBuffers(1, &m_VBO);
}

CBlock *CBlock::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CBlock();
		}
		catch (...) {
			return NULL;
		}
	}
	
	return m_instance;
}

void CBlock::Destroy(void)
{
	m_instance = NULL;
	delete this;
}

int CBlock::Load(void)
{
#if !defined(_OLD_GL)
	m_offsetId = glGetAttribLocation(CShader::GetInstance()->Program(), "offset");
	cout << "offset index: " << m_offsetId << endl;
	if (m_offsetId >= 0)
		glEnableVertexAttribArray(m_offsetId);
	CVertices::GetInstance()->BindVAO();

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	StatusPrint();
	glBufferData(GL_ARRAY_BUFFER, sizeof(*m_offset) * m_iCount, m_offset, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribPointer(m_offsetId, 4, GL_FLOAT, GL_FALSE,
		sizeof(float) * 4,
		0);

	glVertexAttribDivisor(m_offsetId, 1);
	CVertices::GetInstance()->UnbindVAO();
#else
	m_offsetId = glGetUniformLocation(CShader::GetInstance()->Program(), "offset");
	cout << "offset index: " << m_offsetId << endl;
#endif

	m_isBlockId = glGetUniformLocation(CShader::GetInstance()->Program(), "isBlock");
	cout << "isBlock index: " << m_isBlockId << endl;

	m_loaded = true;
	return 0;
}

int CBlock::UpdateOffset(int index)
{
	glUniform4f(m_offsetId, m_offset[index].x, m_offset[index].y, m_offset[index].z, m_offset[index].w);
	StatusPrint();
	return 0;
}

int CBlock::Render(void)
{
	mat4 mvp;

	mvp = CPerspective::GetInstance()->Matrix() * CView::GetInstance()->Matrix() * CModel::GetInstance()->Matrix();

	glUniformMatrix4fv(CShader::GetInstance()->MVPId(), 1, GL_TRUE, (const GLfloat *)mvp);
	StatusPrint();

	glUniform1i(m_isBlockId, 1);

	// Drawing blocks
#if !defined(_OLD_GL)
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, 0, m_iCount);
	StatusPrint();
#else
	int i;

	for (i = 0; i < m_iCount; i++) {
		UpdateOffset(i);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, 0);
		StatusPrint();
	}
#endif
	glUniform1i(m_isBlockId, 0);

	return 0;
}

/* End of a file */
