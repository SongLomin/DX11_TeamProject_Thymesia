#pragma once
#include "Client_Defines.h"
#include <stdarg.h>


BEGIN(Engine)

class CTransform;

END


BEGIN(Client)


class CTransformUtils
{
public:
	/*
	*	넘겨준 행렬의 곱을 리턴하는 함수
		인자1. 행렬의 개수.
		인자2. 넘길 행렬들
	*/
	static _matrix Mul_Matrix(_int imatCount, _fmatrix mat ...);

};

END
