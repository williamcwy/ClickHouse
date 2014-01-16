#pragma once

#include <Poco/SharedPtr.h>

#include <DB/DataStreams/IProfilingBlockInputStream.h>
#include <DB/Columns/ColumnConst.h>
#include <DB/Core/ColumnWithNameAndType.h>

namespace DB
{

/** Добавляет в блок const column с заданным значением.
  */
template <typename ColumnType>
class AddingConstColumnBlockInputStream : public IProfilingBlockInputStream
{
public:
	AddingConstColumnBlockInputStream(
		BlockInputStreamPtr input_,
		DataTypePtr data_type_,
		ColumnType value_,
		String column_name_)
		: data_type(data_type_), value(value_), column_name(column_name_)
	{
		std :: cerr << column_name << " " << value << std :: endl;
		children.push_back(input_);
	}

	String getName() const { return "AddingConstColumnBlockInputStream"; }

	String getID() const
	{
		std::stringstream res;
		res << "AddingConstColumn(" << children.back()->getID() << ")";
		return res.str();
	}

protected:
	Block readImpl()
	{
		Block res = children.back()->read();
		if (!res)
			return res;
		ColumnPtr column_ptr = new ColumnConst<ColumnType> (res.rows(), value, data_type);
		ColumnWithNameAndType column(column_ptr, data_type, column_name);

		res.insert(column);
		return res;
	}

private:
	DataTypePtr data_type;
	ColumnType value;
	String column_name;
};

}
