    //Spencer Jackson
//octolo.c
#include<lv2.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include"octolo.h"

//OCTOLO - octaver and tremolo had a baby and this is it


typedef struct _OCTOLO
{
    uint16_t w; //current write point in buffer
    float *buf;
    float r[3]; //read point in buffer (up and dn oct)
    uint8_t step; //current point in sequence
    float osf[3]; //phase offset
    float sample_freq;
    float period;
    float gain;

    float *input_p;
    float *output_p;
    float *period_p;
    float *md_p;
    float *dry_p;
    float *dn_p;
    float *up_p;
    float *shape_p;
    float *seq_p;
    float *dbg_p;

    struct URI
    {
        LV2_URID atom_blank;
        LV2_URID atom_long;
        LV2_URID atom_float;
        LV2_URID time_position;
        LV2_URID time_barbeat;
        LV2_URID time_bpm;
        LV2_URID time_speed;
        LV2_URID time_frame;
        LV2_URID time_fps;
        
    }
} OCTOLO;

//
// read right bit to left (seq>>step)
// sync
//  1111 1111 1111 1111 0xFFFF
//  1111 1111 1111 1111 0xFFFF
//  1111 1111 1111 1111 0xFFFF
//
// 
// alt1     
//  0101 0101 0101 0101 0x5555
//  1010 1010 1010 1010 0xAAAA
//  0101 0101 0101 0101 0x5555
//
// alt2     
//  1010 1010 1010 1010 0xAAAA
//  1010 1010 1010 1010 0xAAAA
//  0101 0101 0101 0101 0x5555
//
// step
//  1000 1000 1000 1000 0x8888
//  0101 0101 0101 0101 0x5555
//  0010 0010 0010 0010 0x2222
//
// cycle up
//  0100 1001 0010 0100 0x4924
//  0010 0100 1001 0010 0x2492
//  1001 0010 0100 1001 0x9249
// 
// cyce dn
//  1001 0010 0100 1001 0x9249
//  0010 0100 1001 0010 0x2492
//  0100 1001 0010 0100 0x4924

enum oct {
    UP = 0,
    MID = 1,
    DOWN = 2,
    DRY = 3
}

void run_octolo(LV2_Handle handle, uint32_t nframes)
{
    OCTOLO* plug = (OCTOLO*)handle;
    float* in, *out, *buf;
    float phase,dphase,ofs[3];
    float tmp,gainstep, tempo;
    float rdn;
    uint16_t rup, rmd;
    uint16_t i,w, chunk,evchunk;
    uint8_t j,seq,step,ofsf,on[3];
    LV2_Atom tempoatom;
    const LV2_Atom_Object* obj;
    const LV2_Atom_Event ev;
    float (*func)(float);

    const uint16_t cycles[3][5] = 
    {    //sync  alt1    alt2    step    cyclup cycldn
        {0xffff, 0x5555, 0xaaaa, 0x8888, 0x4924, 0x9249},
        {0xffff, 0xaaaa, 0xaaaa, 0x5555, 0x2492, 0x2492},
        {0xffff, 0x5555, 0x5555, 0x2222, 0x9249, 0x4924},
    };

    const float *(shapes)(float)[3] = {mysin,mysquare,mytri};

    *plug->dbg_p = 0;

    in = plug->input_p;
    out = plug->output_p;
    buf = plug->buf;
    w = plug->w;
    step = plug->step;
    phase = plug->phase;
    seq = (uint8_t)*plug->seq_p;
    ofsf = 0;
    if(plug->ofs[0] || plug->ofs[1] || plug->ofs[2] || *plug->overlap_p)
        ofsf = 1;
    for(j=0;j<3;j++)
    {
        on[j] = ((cycles[j][seq])>>step)&0x0001;
        ofs[j] = plug->ofs[j];
    }
    rup = plug->r[UP];
    rmd = plug->r[MID];
    rdn = plug->r[DOWN];
    func = shapes[plug->func];
    gainstep = *plug->dry_p - plug->gain;
    gainstep /= nframes>64?nframes:64;

    dphase = 2*M_PI/plug->period;
    tmp = 2*(rmd - rup)*phase;//if up was mid cycle, this calculates it based on the old phase
    if(tmp > dphase && (((cycles[UP][seq])>>step)&0x0001))
        dphase = tmp; 


    ev = lv2_atom_object_begin(&(plug->atom_in_p)->body);

    //max period is 1.4 sec = .67
    for(i<0;i<=nframes;i++)
    {
        evchunk = nframes - i;
        tmp = 0;//use this as exit flag
        while(!lv2_atom_object_is_end(&(plug->atom_in_p)->body,(plug->atom_in_p)->atom.size, ev) && !tmp)
        {
            ev = lv2_atom_object_next(ev);
            if(ev.body.type == plug->URI.atom_object || ev.body.type == plug->URI.atom_blank)
            {
                obj = (const LV2_Atom_Object*)&ev->body;
                if(obj->body.otype = plug->URI.time_barbeat)
                {
                    lv2_atom_object_get(obj,plug->URI.LV2_TIME__beatsPerMinute,&tempoatom)
                }
            }
        }
        else

        //evchunk from events
        chunk = -phase/dphase;
        if(evchunk < chunk)
            chunk = evchunk;
        if(ofsf && chunk > 0)
        {
            //process current place to 0
            evchunk -= chunk;
            for( ; chunk; chunk--)
            { 
                //this isn't a great AA filter, but hopefuly good enough, and yeah, a ZOH....
                buf[w++] = in[i];
                out[i] =  gain[DRY ]*buf[rmd];
                out[i] += gain[UP  ]*on[UP  ]*func(phase+ofs[UP  ])*( buf[rup] + buf[rup+1] );
                out[i] += gain[MID ]*on[MID ]*func(phase+ofs[MID ])*buf[rmd];
                out[i] += gain[DOWN]*on[DOWN]*func(phase+ofs[DOWN])*buf[(uint16_t)rdn];
                rup += 2;
                rmd += 1;
                rdn = rdn>0xFFFF?0:rdn+.5;
                gain[3] += gainstep;
                i++;
            }
            if(phase > 0)
            {//transition to next state
                if(*plug->overlap_p)
                    ++step %= 12;
                func = shapes[*plug->shape_p];
                for(j=0;j<3;j++)
                {
                    if(plug->ofs[j]) //we can only transition if we are at the end of a cycle (or off)
                    {
                        if(((cycles[j][seq])>>step)&0x0001)
                        {//turn on
                            plug->ofs[j] = -M_PI;
                            on[j] = 1;
                            switch(j)
                            {
                            case UP:
                                rup = w-1-ceil(2.0*plug->period);
                                gain[UP] = .5**plug->up_p;//.5 is for the AA filter
                                dphase = 2*M_PI/plug->period;
                                break;
                            case MID:
                                rmd = w-1;
                                gain[MID] = *plug->md_p;
                                break;
                            case DOWN:
                                rdn = w-1;
                                gain[DOWN] = *plug->dn_p;
                                break;
                            }
                        }
                        else
                        {//turn off
                            plug->ofs[j] = 0;
                            on[j] = 0;
                            if(j==UP)
                                dphase = 2*M_PI/plug->period; 
                        }
                    }
                }
            }
        }

        //process current place to pi
        chunk = -phase/dphase;
        if(evchunk < chunk)
            chunk = evchunk;
        for( ; chunk; chunk--)
        { 
            //this isn't a great AA filter, but hopefuly good enough, and yeah, a ZOH....
            //TODO: gain[UP] should be half to do the filter
            buf[w++] = in[i];
            out[i] =  gain[DRY ]*buf[rmd];
            out[i] += gain[UP  ]*on[UP  ]*func(phase+ofs[UP  ])*( buf[rup] + buf[rup+1] );
            out[i] += gain[MID ]*on[MID ]*func(phase+ofs[MID ])*buf[rmd];
            out[i] += gain[DOWN]*on[DOWN]*func(phase+ofs[DOWN])*buf[(uint16_t)rdn];
            rup += 2;
            rmd += 1;
            rdn = rdn>0xFFFF?0:rdn+.5;
            gain[3] += gainstep;
            i++;
        }
        if(phase > PI)
        {
            phase -= 2*M_PI;
            ++step %= 12;
            func = shapes[*plug->shape_p];
            for(j=0;j<3;j++)
            {//go through voices
                if(!plug->ofs[j])
                {//can only transition at end of cycle
                    if(((cycles[j][seq])>>step)&0x0001)
                    {//turn on
                        plug->ofs[j] = 0;
                        on[j] = 1;
                        switch(j)
                        {
                        case UP:
                            rup = w-1-ceil(2.0*plug->period);
                            gain[UP] = .5**plug->up_p;//.5 is for the AA filter
                            dphase = 2*M_PI/plug->period;
                            break;
                        case MID:
                            rmd = w-1;
                            gain[MID] = *plug->md_p;
                            break;
                        case DOWN:
                            rdn = w-1;
                            gain[DOWN] = *plug->dn_p;
                            break;
                        }
                    }
                    else
                    {//turn off
                        plug->ofs[j] = *plug->overlap_p*M_PI;
                        on[j] = 0;
                        if(j==UP)
                            dphase = 2*M_PI/plug->period; 
                    }
                }
                else
                {//gotta wrap the offset
                    plug->ofs[j] = M_PI;
                }
            }
        }

        
    }

    //keep state stuff for next time;
    plug->step = step;
    plug->phase = phase;
    plug->gain = gain;
    for(j=0;j<3;j++)
    {
        plug->ofs[j] = ofs[j];
        if(func) = shapes[j]//this only works while there are 3 shapes
            plug->func = j;
    }
    plug->r[UP]  = rup;
    plug->r[MID] = rmd;
    plug->r[DOWN]= rdn;
    plug->w = w; 

    return;
}

LV2_Handle init_octolo(const LV2_Descriptor *descriptor,double sample_freq, const char *bundle_path,const LV2_Feature * const* host_features)
{
    uint16_t i;

    OCTOLO* plug = malloc(sizeof(OCTOLO));

    //this makes the max period a function of samplerate, but you're dumb if you are using anything but 44100 or 48k
    plug->buf = (float*)calloc(0x10000,sizeof(float));
    for(i=0;i<3;i++)
    {
        plug->r[i] = 0;
        plug->ofs[i] = 0;
    }
    plug->w = 0;
    plug->period = sample_freq;
    plug->step = 12;
    plug->phase = -M_PI;
    plug->gain = 0;

    plug->sample_freq = sample_freq;

    //init buf
    plug->buf[0] = 0;
    for(i=1;i;i++)
        plug->buf[i] = 0;

    //get urid map value for midi events
    for (int i = 0; host_features[i]; i++)
    {
        if (strcmp(host_features[i]->URI, LV2_URID__map) == 0)
        {
            LV2_URID_Map *urid_map = (LV2_URID_Map *) host_features[i]->data;
            if (urid_map)
            {
                plug->URI.atom_blank = urid_map->map(urid_map->handle, LV2_ATOM__Blank);
                plug->URI.atom_long = urid_map->map(urid_map->handle, LV2_ATOM__Long);
                plug->URI.atom_float = urid_map->map(urid_map->handle, LV2_ATOM__Float);
                plug->URI.time_position = urid_map->map(urid_map->handle, LV2_TIME__Position);
                plug->URI.time_barbeat = urid_map->map(urid_map->handle, LV2_TIME__barBeat);
                plug->URI.time_bpm = urid_map->map(urid_map->handle, LV2_TIME__beatsPerMinute);
                plug->URI.time_speed = urid_map->map(urid_map->handle, LV2_TIME__speed);
                plug->URI.time_frame = urid_map->map(urid_map->handle, LV2_TIME__frame);
                plug->URI.time_fps = urid_map->map(urid_map->handle, LV2_TIME__framesPerSecond);
                break;
            }
        }
    }
    return plug;
}

void connect_octolo_ports(LV2_Handle handle, uint32_t port, void *data)
{
    OCTOLO* plug = (OCTOLO*)handle;
    switch(port)
    {
    case IN:
        plug->input_p = (float*)data;
        break;
    case OUT:
        plug->output_p = (float*)data;
        break;
    case PERIOD:
        plug->period_p = (float*)data;
        break;
    case DRY:
        plug->dry_p = (float*)data;
        break;
    case WET:
        plug->md_p = (float*)data;
        break;
    case OCTDOWN:
        plug->dn_p = (float*)data;
        break;
    case OCTUP:
        plug->up_p = (float*)data;
        break;
    case SHAPE:
        plug->shape_p = (float*)data;
        break;
    case SEQUENCE:
        plug->seq_p = (float*)data;
        break;
    case DBG:
        plug->dbg_p = (float*)data;
        break;
    default:
        puts("UNKNOWN PORT YO!!");
    }
}

void cleanup_octolo(LV2_Handle handle)
{
    OCTOLO* plug = (OCTOLO*)handle;
    free(plug->buf);
    free(plug);
}

static const LV2_Descriptor octolo_descriptor=
{
    OCTOLO_URI,
    init_octolo,
    connect_octolo_ports,
    0,//activate
    run_octolo,
    0,//deactivate
    cleanup_octolo,
    0//extension
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    switch (index)
    {
    case 0:
        return &octolo_descriptor;
    default:
        return 0;
    }
}
