#include "stdafx.h"
#include "TransformUtils.h"
#include "Transform.h"
#include <stdarg.h>
#include <stdio.h>


typedef char* va_list;

_matrix CTransformUtils::Mul_Matrix(_int imatCount, _matrix mat ...)
{
	va_list list;
	va_list copy;


	_matrix muledMat;//곱해지는 매트릭스
	_matrix matTemp;

	muledMat = XMMatrixIdentity();

	va_start(list, imatCount);
	va_copy(copy, list);

	for (_int i = 0; i != imatCount; ++i)
	{
		matTemp = va_arg(copy, _matrix);

		muledMat *= matTemp;
	}

	va_end(list);
	va_end(copy);

	return muledMat;
}
