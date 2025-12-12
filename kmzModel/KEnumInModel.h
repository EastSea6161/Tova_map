#pragma once

namespace KReturn
{
	enum KEMReturn
	{
		R_OK = 1,
		R_ERROR,
		R_AMBIGUOUS,
		R_INPUT_ERROR
	};
}

namespace KEMDistribution
{
	enum KEMPATYPE
	{
		K_PRODUCTION = 1,
		K_ATTRACTION 
	};
};

namespace KEMModeChoice
{
    enum NODETYPE
    {
        MODE_NODE_TYPE   = 1,
        NESTED_NODE_TYPE = 6 
    };    
}