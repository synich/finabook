#include "fina.h"

extern sqlite3* db;

//callback from SQLite and redraw UI, can't decouple model and view now...
static int cb_select(void* v, int count, char** value, char** name){
	FinaTable* table = (FinaTable*)v;
	table->setValue(value, count);
	table->rows(++table->row);
	return 0;
}

//Callback for input SQL statement, and execute.
//Notice:SQL execute also need a callback
int cb_sqlselect(const char* input, sel_param* v, int kind){
	char** errmsg = NULL;
	char buff[256] = {0};
	static int page = 0;

	page = page + kind;
	page<0 ? page = 0: NULL;	
	sprintf(buff, "%s limit %d offset %d", input, SHOWROW, page * SHOWROW);

	int ret = sqlite3_exec(db, buff, cb_select, v->table, errmsg);
	return ret;
}
static int count_symbol(const char* input, char symbol){
	int count = 0;
	while(*input){
		if(*input == symbol) ++count;
		++input;
	}
	return count;
}

int cb_sqlinsert(const char* input, void* v){
	char** errmsg = NULL;
	char buff[256] = {0};
	struct tm *t;
	time_t now;
	now = time(NULL);
	t = localtime(&now);
	
	int comnum = count_symbol(input, ',');
	if(2 == comnum){//Defalu 'type', num, 'memo'
	  sprintf(buff, "insert into y%dh%d values(NULL, %d, %d, %s, '');",
	    t->tm_year-100, t->tm_mon/6+1, t->tm_mon+1, t->tm_mday, input);
	}
	else if(3 == comnum){//Add optional
	  sprintf(buff, "insert into y%dh%d values(NULL, %d, %d, %s);",
	    t->tm_year-100, t->tm_mon/6+1, t->tm_mon+1, t->tm_mday, input);
	}
	else if(4 == comnum){//Input date, and no optional
	  int mon = atoi(input); //This actual month, so change calulate season method
	  sprintf(buff, "insert into y%dh%d values(NULL, %s, '');",
	    t->tm_year-100, (mon+5)/6, input);
	}
	else if(5 == comnum){//Input date and optional
	  int mon = atoi(input);
	  sprintf(buff, "insert into y%dh%d values(NULL, %s);",
	    t->tm_year-100, (mon+5)/6, input);
	}
	int ret = sqlite3_exec(db, buff, NULL, NULL, errmsg);
	if(SQLITE_OK != ret){
		return FB_INSERTERR;
	}
	return FB_OK;
}

void cb_sqlclose(void* v){
	sqlite3_close((sqlite3*)v);
	exit(0);
}
