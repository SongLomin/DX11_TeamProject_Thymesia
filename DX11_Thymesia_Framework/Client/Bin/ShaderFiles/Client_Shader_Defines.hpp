

sampler		DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler		ClampSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

sampler		BorderSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = border;
	AddressV = border;
};


sampler		PointSampler = sampler_state
{
	filter = min_mag_mip_point;
	AddressU = wrap;
	AddressV = wrap;
};

sampler		PointClampSampler = sampler_state
{
	filter = min_mag_mip_point;
	AddressU = clamp;
	AddressV = clamp;
};



RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

RasterizerState RS_Wireframe
{
	FillMode = wireframe;
	CullMode = back;
	FrontCounterClockwise = false;
};

RasterizerState RS_Sky
{
	FillMode = solid;
	CullMode = front;
	FrontCounterClockwise = false;
};

RasterizerState RS_NonCulling
{
	FillMode = solid;
	CullMode = none;
	FrontCounterClockwise = false;
};

DepthStencilState	DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState	DSS_None_ZTest_And_Write
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

DepthStencilState	DSS_ZTest_And_No_Write
{
	DepthEnable = true;
	DepthWriteMask = zero;
};

BlendState BS_None
{
	BlendEnable[0] = false;
};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = true;
	SrcBlend[0] = SRC_ALPHA;
	DestBlend[0] = INV_SRC_ALPHA;
	BlendOp[0] = Add;
};

BlendState BS_One
{
	BlendEnable[0] = true;
	SrcBlend[0] = ONE;
	DestBlend[0] = ONE;
	BlendOp[0] = Add;
};
