/*
 *                           0BSD 
 * 
 *                    BSD Zero Clause License
 * 
 *  Copyright (c) 2019 Hermann Meyer
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.

 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */


#include <lv2/core/lv2.h>
#include <lv2/ui/ui.h>
#include <lv2/state/state.h>
#include <lv2/worker/worker.h>
#include <lv2/atom/atom.h>
#include <lv2/options/options.h>
#if defined USE_ATOM || defined USE_MIDI
#include <lv2/atom/util.h>
#include <lv2/atom/forge.h>
#include <lv2/midi/midi.h>
#include <lv2/urid/urid.h>
#include <lv2/patch/patch.h>
#endif

// xwidgets.h includes xputty.h and all defined widgets from Xputty
#include "xwidgets.h"
#include "xfile-dialog.h"

#pragma once

#ifndef LV2_PLUGIN_H_
#define LV2_PLUGIN_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int stride;
    int width;
    int height;
    unsigned char* data;
} CairoImageData;

typedef int PortIndex;

#ifdef USE_MIDI
enum {
    _FULL          = 1<<0,
    _EMPTY         = 1<<1,
};

typedef struct {
    int send_cc[25];
    uint8_t cc_num[25];
    uint8_t pg_num[25];
    uint8_t bg_num[25];
    uint8_t me_num[25];
} MidiMessenger;
#endif

// main window struct
typedef struct {
#ifdef USE_MIDI
    LV2_URID midi_MidiEvent;
    LV2_URID atom_eventTransfer;

    LV2_Atom midiatom; 
    LV2_Atom_Forge forge;
    MidiMessenger mm;
    int midi_port;
#endif
    void *parentXwindow;
    Xputty main;
    Widget_t *win;
    Widget_t *widget[CONTROLS];
    Widget_t *elem[GUI_ELEMENTS];
    Widget_t *tab_elem[TAB_ELEMENTS];
    void *private_ptr;
    int need_resize;
    LV2_URID_Map* map;
    void *controller;
    LV2UI_Write_Function write_function;
    LV2UI_Resize* resize;
} X11_UI;

#ifdef USE_MIDI
void messenger_init(MidiMessenger *mm);

bool send_midi_cc(MidiMessenger *mm, uint8_t _cc, const uint8_t _pg,
                            const uint8_t _bgn, const uint8_t _num);

int next(MidiMessenger *mm, int i);

void fill(MidiMessenger *mm, unsigned char *midi_send, const int i);

void set_midi_port(X11_UI *ui, int p) {ui->midi_port = p;}
#endif

// controller value changed, forward value to host when needed
void plugin_value_changed(X11_UI *ui, Widget_t *w, PortIndex index);

// set the plugin initial window size
void plugin_set_window_size(int *w,int *h,const char * plugin_uri);

// set the plugin name
const char* plugin_set_name();

// create all needed controller 
void plugin_create_controller_widgets(X11_UI *ui, const char * plugin_uri);


Widget_t* add_lv2_knob(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_combobox(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_vmeter(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_hmeter(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_vslider(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_hslider(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_toggle_button(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_image_toggle(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_button(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_image_button(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_valuedisplay(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_label(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                 X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_frame(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_image(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_waveview(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_tabbox(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_tab(Widget_t *w, Widget_t *p, PortIndex index, const char * label, X11_UI* ui);

Widget_t* add_lv2_file_button(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height);

Widget_t* add_lv2_midikeyboard(Widget_t *w, Widget_t *p, PortIndex index, const char * label,
                                X11_UI* ui, int x, int y, int width, int height);

void load_bg_image(X11_UI* ui, const char* image);

void load_controller_image(Widget_t* w, const char* image);

// free used mem on exit
void plugin_cleanup(X11_UI *ui);

// controller value changed message from host
void plugin_port_event(LV2UI_Handle handle, uint32_t port_index,
                        uint32_t buffer_size, uint32_t format,
                        const void * buffer);
#ifdef __cplusplus
}
#endif

#endif //LV2_PLUGIN_H_
