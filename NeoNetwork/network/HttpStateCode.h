#pragma once

namespace neo::network
{
	enum class HttpStateCode : int
	{
		OK = 200,
		Created =201,
		Accepted =202,
		NoContect=204,




		BadRequest=400,
		NotFound=404,

	};
}