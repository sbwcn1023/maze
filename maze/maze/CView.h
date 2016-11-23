#pragma once
#if !defined(__CVIEW_H)
#define __CVIEW_H

class CView {
private:
	// Camera info
	vec3 m_eye;
	vec3 m_at;
	vec3 m_up;
	bool m_updated;

	mat4 m_viewMatrix;

	vec3 m_rotateAxis;
	float m_rotateAngle;

	vec3 m_translate;

	static CView *m_instance;

	CView(void);
	virtual ~CView(void);

public:
	static CView *GetInstance(void);
	void Destroy(void);

	void SetEye(vec3 &eye);
	void SetEye(float x, float y, float z);

	void SetAt(vec3 &at);
	void SetAt(float x, float y, float z);

	void SetUp(vec3 &up);
	void SetUp(float x, float y, float z);

	vec3 Eye(void);
	vec3 At(void);
	vec3 Up(void);


	mat4 Matrix(void);

	bool Updated(void);
};

#endif
/* End of a file */