
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cstring>
#include <unistd.h>

#include <lv2/core/lv2.h>
///////////////////////// MACRO SUPPORT ////////////////////////////////

#define PLUGIN_URI "urn:bill:monospace"

using std::min;
using std::max;

#define __rt_data __attribute__((section(".rt.data")))
#include "monospace.cc"

////////////////////////////// PLUG-IN CLASS ///////////////////////////

namespace monospace {

class Xmonospace
{
private:
    float* input0;
    float* output0;
    float* output1;
    monospace::Dsp* plugin;

    // private functions
    inline void run_dsp_(uint32_t n_samples);
    inline void connect_(uint32_t port,void* data);
    inline void init_dsp_(uint32_t rate);
    inline void connect_all__ports(uint32_t port, void* data);
    inline void activate_f();
    inline void clean_up();
    inline void deactivate_f();
public:
    // LV2 Descriptor
    static const LV2_Descriptor descriptor;
    // static wrapper to private functions
    static void deactivate(LV2_Handle instance);
    static void cleanup(LV2_Handle instance);
    static void run(LV2_Handle instance, uint32_t n_samples);
    static void activate(LV2_Handle instance);
    static void connect_port(LV2_Handle instance, uint32_t port, void* data);
    static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
                                double rate, const char* bundle_path,
                                const LV2_Feature* const* features);
    Xmonospace();
    ~Xmonospace();
};

// constructor
Xmonospace::Xmonospace() :

    input0(NULL),
    output0(NULL),
    output1(NULL),
    plugin(monospace::plugin()) {};

// destructor
Xmonospace::~Xmonospace() {
    plugin->del_instance(plugin);
};

///////////////////////// PRIVATE CLASS  FUNCTIONS /////////////////////

void Xmonospace::init_dsp_(uint32_t rate)
{
    plugin->init(rate);
}

// connect the Ports used by the plug-in class
void Xmonospace::connect_(uint32_t port,void* data)
{
    switch ((PortIndex)port)
    {
        case 0:
            input0 = static_cast<float*>(data);
            break;
        case 1:
            output0 = static_cast<float*>(data);
            break;
        case 2:
            output1 = static_cast<float*>(data);
            break;
        default:
            break;
    }
}

void Xmonospace::activate_f()
{
    // allocate the internal DSP mem
}

void Xmonospace::clean_up()
{
    // delete the internal DSP mem
}

void Xmonospace::deactivate_f()
{
    // delete the internal DSP mem
}

void Xmonospace::run_dsp_(uint32_t n_samples)
{
    if(n_samples<1) return;

    plugin->compute(n_samples, input0, output0, output1);
}

void Xmonospace::connect_all__ports(uint32_t port, void* data)
{
    // connect the Ports used by the plug-in class
    connect_(port,data); 
    plugin->connect(port,data);}

////////////////////// STATIC CLASS  FUNCTIONS  ////////////////////////

LV2_Handle 
Xmonospace::instantiate(const LV2_Descriptor* descriptor,
                            double rate, const char* bundle_path,
                            const LV2_Feature* const* features)
{
    // init the plug-in class
    Xmonospace *self = new Xmonospace();
    if (!self) {
        return NULL;
    }
    self->init_dsp_((uint32_t)rate);
    return (LV2_Handle)self;
}

void Xmonospace::connect_port(LV2_Handle instance, 
                                   uint32_t port, void* data)
{
    // connect all ports
    static_cast<Xmonospace*>(instance)->connect_all__ports(port, data);
}

void Xmonospace::activate(LV2_Handle instance)
{
    // allocate needed mem
    static_cast<Xmonospace*>(instance)->activate_f();
}

void Xmonospace::run(LV2_Handle instance, uint32_t n_samples)
{
    // run dsp
    static_cast<Xmonospace*>(instance)->run_dsp_(n_samples);
}

void Xmonospace::deactivate(LV2_Handle instance)
{
    // free allocated mem
    static_cast<Xmonospace*>(instance)->deactivate_f();
}

void Xmonospace::cleanup(LV2_Handle instance)
{
    // well, clean up after us
    Xmonospace* self = static_cast<Xmonospace*>(instance);
    self->clean_up();
    delete self;
}

const LV2_Descriptor Xmonospace::descriptor =
{
    PLUGIN_URI ,
    Xmonospace::instantiate,
    Xmonospace::connect_port,
    Xmonospace::activate,
    Xmonospace::run,
    Xmonospace::deactivate,
    Xmonospace::cleanup,
    NULL
};

} // end namespace monospace

////////////////////////// LV2 SYMBOL EXPORT ///////////////////////////

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    switch (index)
    {
        case 0:
            return &monospace::Xmonospace::descriptor;
        default:
            return NULL;
    }
}

///////////////////////////// FIN //////////////////////////////////////
