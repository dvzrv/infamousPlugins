//spencer jackson
//octolo and stereoctolo UIs

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tikloo/tk.h"
#include"octolo.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
#include"Library_3_am.h"

#define OCTOLO_UI_URI "http://ssj71.github.io/infamousPlugins/plugs.html#octolo_ui"

typedef struct 
{
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
    uint8_t map[OVERLAP+1];
}shared_t;

typedef struct
{
    uint16_t id;
    float v;
}baggage_t;


void lv2dial_callback(tk_t tk, const PuglEvent* ev, uint16_t n)
{
    shared_t* s = (shared_t*)tk->user[0];
    baggage_t* b = (baggage_t*)tk->user[n];
    b->v = tk_getdial(tk,n);
    s->write_function(s->controller,b->id,sizeof(float),0,&(b->v));
}

void lv2button_callback(tk_t tk, const PuglEvent* ev, uint16_t n)
{
    shared_t* s = (shared_t*)tk->user[0];
    baggage_t* b = (baggage_t*)tk->user[n];
    b->v = *(bool*)tk->value[n];
    s->write_function(s->controller,b->id,sizeof(float),0,&(b->v));
}

void setup_octolo_ui(tk_t tk)
{
    uint8_t n;
    baggage_t* b;
    shared_t* s = (shared_t*)tk->user[0];

    //length
    n = tk_addaDial(tk, /*xywh*/10,10,80,80, /*min*/.01, /*max*/8, /*dflt*/1);
    b = (baggage_t*)malloc(sizeof(baggage_t));
    b->id = LENGTH;
    tk->user[n] = b;
    tk->callback_f[n] = lv2dial_callback;
    s->map[LENGTH] = n;
    tk_setstring(&tk->tip[n],"Length",0);


    //slope
    n = tk_addaDial(tk, /*xywh*/100,10,80,80, /*min*/0, /*max*/1, /*dflt*/.8);
    b = (baggage_t*)malloc(sizeof(baggage_t));
    b->id = SLOPE;
    tk->user[n] = b;
    tk->callback_f[n] = lv2dial_callback;
    s->map[SLOPE] = n;
    tk_setstring(&tk->tip[n],"Slope",0);

    //overlap
    n = tk_addaButton(tk, /*xywh*/190,20,50,50, /*dflt*/false);
    b = (baggage_t*)malloc(sizeof(baggage_t));
    b->id = OVERLAP;
    tk->user[n] = b;
    tk->callback_f[n] = lv2button_callback;
    s->map[OVERLAP] = n;
    tk_setstring(&tk->tip[n],"Overlap",0);

    //dry
    n = tk_addaDial(tk, /*xywh*/10,100,50,50, /*min*/0, /*max*/1, /*dflt*/.5);
    b = (baggage_t*)malloc(sizeof(baggage_t));
    b->id = DRYG;
    tk->user[n] = b;
    tk->callback_f[n] = lv2dial_callback;
    s->map[DRYG] = n;
    tk_setstring(&tk->tip[n],"Dry Gain",0);

    //wet
    n = tk_addaDial(tk, /*xywh*/70,100,50,50, /*min*/0, /*max*/1, /*dflt*/.5);
    b = (baggage_t*)malloc(sizeof(baggage_t));
    b->id = WET;
    tk->user[n] = b;
    tk->callback_f[n] = lv2dial_callback;
    s->map[WET] = n;
    tk_setstring(&tk->tip[n],"Wet Gain",0);

    //down
    n = tk_addaDial(tk, /*xywh*/130,100,50,50, /*min*/0, /*max*/1, /*dflt*/.5);
    b = (baggage_t*)malloc(sizeof(baggage_t));
    b->id = OCTDOWN;
    tk->user[n] = b;
    tk->callback_f[n] = lv2dial_callback;
    s->map[OCTDOWN] = n;
    tk_setstring(&tk->tip[n],"Oct. Down Gain",0);

    //up
    n = tk_addaDial(tk, /*xywh*/190,100,50,50, /*min*/0, /*max*/1, /*dflt*/.5);
    b = (baggage_t*)malloc(sizeof(baggage_t));
    b->id = OCTUP;
    tk->user[n] = b;
    tk->callback_f[n] = lv2dial_callback;
    s->map[OCTUP] = n;
    tk_setstring(&tk->tip[n],"Oct. Up Gain",0);

    //seq
    n = tk_addaDial(tk, /*xywh*/10,160,50,50, /*min*/0, /*max*/5, /*dflt*/0);
    b = (baggage_t*)malloc(sizeof(baggage_t));
    b->id = SEQUENCE;
    tk->user[n] = b;
    tk->callback_f[n] = lv2dial_callback;
    s->map[SEQUENCE] = n;
    tk_setstring(&tk->tip[n],"Sequence",0);

    tk_addaTooltip(tk,0);

    tk_font_stuff* f = tk_gimmeaFont(tk,
                            Library_3_am_otf, // font data
                            sizeof(Library_3_am_otf), // size
                            0, // index
                            72); // height
    tk_addaText(tk, /*xywh*/10,220,230,10, /*font*/f, /*msg*/"the infamous");
    tk_addaText(tk, /*xywh*/10,300,230,70, /*font*/f, /*msg*/"OCToLO");
    //TODO: need a way to make the title auto-scale

}

static LV2UI_Handle init_octolo_ui(
    const struct _LV2UI_Descriptor * descriptor,
    const char * plugin_uri,
    const char * bundle_path,
    LV2UI_Write_Function write_function,
    LV2UI_Controller controller,
    LV2UI_Widget * widget,
    const LV2_Feature * const * features
    ) 
{
    tk_t tk;
    if(strcmp(plugin_uri, OCTOLO_URI) != 0)
    {
        return 0;
    }

    intptr_t parentWindow = 0;
    LV2UI_Resize* resize = NULL;
    for (int i = 0; features[i]; ++i)
    {
        if (!strcmp(features[i]->URI, LV2_UI__parent)) 
        {
           parentWindow = (intptr_t)features[i]->data;
        }
        else if (!strcmp(features[i]->URI, LV2_UI__resize)) 
        {
           resize = (LV2UI_Resize*)features[i]->data;
        }
    }
    if(!parentWindow) return 0;

    tk = tk_gimmeaTiKloo( 250, //w
                          444, //h
                          (char*)"the infamous octolo", //title
                          parentWindow); //parent
    if(!tk) return 0;

    if (resize)
    {
        resize->ui_resize(resize->handle, tk->w[0], tk->h[0]);
    }

    shared_t* s = (shared_t*)calloc(1,sizeof(shared_t));
    s->controller = controller;
    s->write_function = write_function;
    tk->user[0] = s;
    setup_octolo_ui(tk);

    *widget = (LV2UI_Widget)tk_embedit(tk);

    return (LV2UI_Handle)tk;
}

void cleanup_octolo_ui(LV2UI_Handle handle)
{
    tk_t tk = (tk_t)handle;
    free(tk->user[0]);
    tk_cleanup(tk);
}

void octolo_ui_port_event(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void * buffer)
{
    tk_t tk = (tk_t)ui;
    if(!format)
    {
      float val = *(float*)buffer;
      uint8_t* map = ((shared_t*)tk->user[0])->map;
      switch(port_index)
      {
      //dials
      case LENGTH:
      case SLOPE:
      case DRYG:
      case WET:
      case OCTDOWN:
      case OCTUP:
      case SEQUENCE:
          tk_setdial(tk,map[port_index],val);
          break;
        //not yet implemented
          break;
      //switch
      case OVERLAP:
          *(bool*)tk->value[map[port_index]] = (bool)val;
          break;
      default:
          break;
      }//switch index
    }//if float
}

static int idle(LV2UI_Handle handle)
{
    tk_t tk = (tk_t)handle;
    tk_idle(tk);
    return 0;
}
static const LV2UI_Idle_Interface idle_iface = { idle };

static const void*
extension_data(const char* uri)
{
  if (!strcmp(uri, LV2_UI__idleInterface))
  {
    return &idle_iface;
  }
  return NULL;
}

static const LV2UI_Descriptor octoloUI_descriptor = {
    OCTOLO_UI_URI,
    init_octolo_ui,
    cleanup_octolo_ui,
    octolo_ui_port_event,
    extension_data
};

LV2_SYMBOL_EXPORT 
const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index) 
{
    switch (index) {
    case 0:
        return &octoloUI_descriptor;
    default:
        return NULL;
    }
}
