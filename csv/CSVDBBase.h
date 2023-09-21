#pragma once

#include <vector>
#include "CSVTable.h"



	class CSVDBBase : public std::vector<std::shared_ptr<CSVTable>> {
	public:
		CSVDBBase() = default;
		virtual ~CSVDBBase() = default;
	};

