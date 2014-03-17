#include "fina.h"
#include "stdio.h"
#include "FL/fl_ask.h"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Output.H"

sqlite3* db;

const char* colname[] = {"id", "mon", "day", "type", "num", "memo", "ext"};

FinaTable::FinaTable(int x, int y, int w, int h, const char* l):Fl_Table(x, y, w, h, l){
	row = 0;
	memset(colvalue, 0, sizeof(colvalue));
	rows(1);
	row_header(1);
	cols(7);
	col_header(1);
	col_width(5, 120);
	end();
}

void FinaTable::draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H){
	char s[64] = {0};
	switch(context){
		case CONTEXT_ROW_HEADER:
		  sprintf(s, "%d", ROW);
		  DrawHeader(s, X, Y, W, H);
		  return;
		case CONTEXT_COL_HEADER:
		  DrawHeader(colname[COL], X, Y, W, H);
		  return;
		case CONTEXT_CELL:
		  DrawData(colvalue[ROW][COL], X, Y, W, H);
		  return;
		default:
		  return;
	}
}

static void dispatch_select(Fl_Widget* w, void* v, int page){
	sel_param *sp = (sel_param*)v;
	const char* input = (sp->input)->value();
	sp->table->row = 0; //Reset & Redraw Table
	int ret = cb_sqlselect(input, sp, page);

	if(ret != SQLITE_OK){
		fl_message("Something wrong in your input! Error code=%d", ret);
	}
	return;	
}
//Using this to decupling UI and Logic, so it's easy to change UI Library
static void cb_btnselect(Fl_Widget* w, void* v){
	dispatch_select(w, v, 0);
}

static void cb_btnprevselect(Fl_Widget* w, void* v){
	dispatch_select(w, v, -1);
}

static void cb_btnnextselect(Fl_Widget* w, void* v){
	dispatch_select(w, v, 1);	
}

static void cb_btninsert(Fl_Widget* w, void* v){
	const char* input = ((Fl_Input*)w)->value();
	int ret = cb_sqlinsert(input, NULL);
	if(ret != SQLITE_OK){
		fl_message("Something wrong in your insert statement! Error code=%d", ret);
	}
	return;
}

static void cb_winclose(Fl_Widget* w, void* v){
	cb_sqlclose(v);
}

int main(int argc, char** argv){
	int ret = sqlite3_open(DBPATH, &db);
	if(ret != SQLITE_OK) return -1;

    Fl_Double_Window win(900, 600, "Finace Table");
    Fl_Input select_input(10, 10, 780, 25);
    Fl_Button prew_sel(800, 10, 40, 25, "prev");
    Fl_Button next_sel(850, 10, 40, 25, "next");
    Fl_Input insert_input(10, 40, 880, 25);
    FinaTable table(10,70,880,510);
    win.end();
    win.callback(cb_winclose, (void*)db);
    win.resizable(table);
    
	sel_param st_sp;
	st_sp.table = &table;
	st_sp.input = &select_input;
    select_input.value("select * from y14h1 where type='income'");
    select_input.when(FL_WHEN_ENTER_KEY_ALWAYS);
    select_input.callback(cb_btnselect, &st_sp);
    prew_sel.callback(cb_btnprevselect, &st_sp);
    next_sel.callback(cb_btnnextselect, &st_sp);
    
    insert_input.value("'food', -100, 'vegetable'");
    insert_input.when(FL_WHEN_ENTER_KEY_ALWAYS);
    insert_input.callback(cb_btninsert, NULL);

    win.show(argc,argv);
    return(Fl::run());
}
