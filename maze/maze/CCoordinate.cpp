#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"
#include "CObject.h"
#include "CShader.h"
#include "CCoordinate.h"
#include "CMovable.h"
#include "CModel.h"
#include "CPerspective.h"
#include "CView.h"

using namespace std;

CCoordinate *CCoordinate::m_instance = NULL;

CCoordinate::CCoordinate(void)
{

}

CCoordinate::~CCoordinate(void)
{

}

CCoordinate *CCoordinate::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CCoordinate();
		}
		catch (...) {
			return NULL;
		}
	}

	return m_instance;
}

void CCoordinate::Destroy(void)
{
	delete this;
	m_instance = NULL;
}

int CCoordinate::Render(void)
{
	GLenum status;
	mat4 mvp;

	mvp = CPerspective::GetInstance()->Matrix() * CView::GetInstance()->Matrix() * CModel::GetInstance()->Matrix();

	glUniformMatrix4fv(CShader::GetInstance()->MVPId(), 1, GL_TRUE, (const GLfloat *)mvp);
	if (glGetError() != GL_NO_ERROR)
		cerr << "Failed to uniform" << endl;

	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (void *)(sizeof(GLfloat) * 36));
	status = glGetError();
	if (status != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << ":" << status << endl;

	return 0;
}

int CCoordinate::Load(void)
{
	return 0;
}

/* End of a file */
