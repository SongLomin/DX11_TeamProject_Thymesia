

////xcopy	/�ɼ�    .�����ؾ��� ���϶Ǵ�����			.������� ����. 

xcopy   /y		..\..\Engine\Bin\Engine.lib			..\..\Reference\Librarys\
xcopy   /y		..\..\Engine\Bin\Engine.dll			..\..\Client\Bin\debug\
xcopy   /y		..\..\Engine\Bin\Engine.dll			..\..\Client\Bin\release\
xcopy   /y		..\..\Engine\ThirdPartyLib\*.lib		..\..\Reference\Librarys\
xcopy   /y/s		..\..\Engine\Public\*.*				..\..\Reference\Headers\
xcopy   /y/s		..\..\Engine\Bin\ShaderFiles\*.*				..\..\Client\Bin\ShaderFiles\