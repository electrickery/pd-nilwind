/* Copyright (c) 2002-2003 krzYszcz and others.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#include <string.h>
#include "m_pd.h"
#include "common/loud.h"
#include "common/grow.h"
#include "sickle/sic.h"
#include "shared.h"

#define FRAMEACCUM_INISIZE  512
#define FRAMEACCUM_NOWRAP   0.f

typedef struct _frameaccum
{
    t_sic     x_sic;
    int       x_size;
    int       x_wrapFlag;
    t_float  *x_frame;
    t_float   x_frameini[FRAMEACCUM_INISIZE];
} t_frameaccum;

static t_class *frameaccum_class;

static t_int *frameaccum_perform(t_int *w)
{
    int nblock = (int)(w[1]);
    t_frameaccum *x = (t_frameaccum *)(w[2]);
    t_float *frame = x->x_frame;
    t_float *in = (t_float *)(w[3]);
    t_float *out = (t_float *)(w[4]);
    
    if (x->x_wrapFlag)
    {   
        while (nblock--)
        { 
            *frame += *in++;
             double dnorm = *frame + SHARED_PI;
             if (dnorm < 0)
                 *frame = SHARED_PI - fmod(-dnorm, SHARED_2PI);
             else
                 *frame = fmod(dnorm, SHARED_2PI) - SHARED_PI;
            *out++ = *frame++;
        }
    } else {
        while (nblock--) *out++ = (*frame++ += *in++); 
    }
    return (w + 5);
}

static void frameaccum_dsp(t_frameaccum *x, t_signal **sp)
{
    int nblock = sp[0]->s_n;
    if (nblock > x->x_size)
	x->x_frame = grow_nodata(&nblock, &x->x_size, x->x_frame,
				 FRAMEACCUM_INISIZE, x->x_frameini,
				 sizeof(*x->x_frame));
    memset(x->x_frame, 0, nblock * sizeof(*x->x_frame));  /* CHECKED */
    dsp_add(frameaccum_perform, 4, nblock, x,
	    sp[0]->s_vec, sp[1]->s_vec);
}

static void frameaccum_free(t_frameaccum *x)
{
    if (x->x_frame != x->x_frameini)
	freebytes(x->x_frame, x->x_size * sizeof(*x->x_frame));
}

static void *frameaccum_new(t_symbol *s, int ac, t_atom *av)
{
    t_frameaccum *x = (t_frameaccum *)pd_new(frameaccum_class);
    int size;
    t_float wrapFlag = FRAMEACCUM_NOWRAP;
    x->x_size = FRAMEACCUM_INISIZE;
    x->x_frame = x->x_frameini;
    if ((size = sys_getblksize()) > FRAMEACCUM_INISIZE)
	x->x_frame = grow_nodata(&size, &x->x_size, x->x_frame,
				 FRAMEACCUM_INISIZE, x->x_frameini,
				 sizeof(*x->x_frame));
                                 
    loud_floatarg(*(t_pd *)x, 0, ac, av, &wrapFlag, 0.f, 1.f, 
        LOUD_CLIP | LOUD_WARN | LOUD_ARGUNDER, 
        LOUD_CLIP | LOUD_WARN | LOUD_ARGOVER, "wrapFlag");
        
    x->x_wrapFlag = (wrapFlag) ? 1 : 0;
    outlet_new((t_object *)x, &s_signal);
    return (x);
}

void frameaccum_tilde_setup(void)
{
    frameaccum_class = class_new(gensym("frameaccum~"),
				 (t_newmethod)frameaccum_new,
				 (t_method)frameaccum_free,
				 sizeof(t_frameaccum), 0, A_GIMME, 0);
    sic_setup(frameaccum_class, frameaccum_dsp, SIC_FLOATTOSIGNAL);
}
