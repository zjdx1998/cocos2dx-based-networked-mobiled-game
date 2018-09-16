#pragma once
#include "item.h"
#include <vector>
class consum :
	public item
{
public:
	consum();
	consum(int, const string &, int, const string &, const string &, int, int,int,int);
	consum(const consum&);
	virtual ~consum();
	int getAmount()const;
	void setAmount(int);
	int getconsum_Type()const;
	void setconsum_Type(int);
	void useItem();

	static vector<consum> initConsumList();

private:
	int amount;
	int consum_Type;
};

