#include "gtest/gtest.h"
//#include "fina.h"
#include "time.h"

#pragma comment(lib, "gtest.lib")

TEST(time, 1){
	struct tm *tvalue;
	time_t t;
	t = time(NULL);
	tvalue = localtime(&t);
	EXPECT_EQ(tvalue->tm_year, 114);//year is offset by 1900
	EXPECT_EQ(tvalue->tm_mon, 1);   //month is offset by 0
	//EXPECT_EQ(tvalue->tm_mday, 22);//day is actual
}

class FinaTable;
//SQL select need seperate page
struct sel_param{
	FinaTable* table;
	int page;
};
static sel_param* update_page(sel_param* param, int num){
	param->page = param->page + num;
	return param;
}

TEST(sql, 1){
	sel_param sp, *psp;
	sp.page = 0;
	psp = update_page(&sp, 1);
	EXPECT_EQ(psp->page, 1);
}
int main(int argc, char** argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}