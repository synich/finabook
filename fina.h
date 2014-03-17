#ifndef __FINA_H__
#define __FINA_H__

#include "sqlite3.h"
#include "time.h"
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>

#define DBPATH "../finace.db"
#define SHOWROW 12
#define FB_OK 0
#define FB_SELECTERR 1
#define FB_INSERTERR 2

class FinaTable;
class Fl_Input;
//SQL select need seperate page
struct sel_param{
	FinaTable* table;
	Fl_Input* input;
};

int cb_sqlselect(const char* input, sel_param* v, int page);
int cb_sqlinsert(const char* input, void* v);
void cb_sqlclose(void* v);

class FinaTable : public Fl_Table {
public:
    FinaTable(int x, int y, int w, int h, const char* l=NULL);
    void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0);
      // Draw the row/col headings
  void DrawHeader(const char *s, int X, int Y, int W, int H) {
    fl_push_clip(X,Y,W,H);
      fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, row_header_color());
      fl_color(FL_BLACK);
      fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
    fl_pop_clip();
  }
  // Draw the cell data
  void DrawData(const char *s, int X, int Y, int W, int H) {
    fl_push_clip(X,Y,W,H);
      // Draw cell bg
      fl_color(FL_WHITE); fl_rectf(X,Y,W,H);
      // Draw cell data
      fl_color(FL_GRAY0); fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
      // Draw box border
      fl_color(color()); fl_rect(X,Y,W,H);
    fl_pop_clip();
  }
  void setValue(char** value, int count){
  	for(int i=0; i<count; ++i){
	  	strcpy(colvalue[row][i], value[i]);
	  }
    return;
  }
  
  int row;
private:
  char colvalue[SHOWROW][7][32];
};

#endif
 