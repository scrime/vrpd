// include flext header
#include <flext/flext.h>
#include <iostream>

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 401)
#error You need at least flext version 0.4.1
#endif

#include <vrpn_Analog.h>

using namespace std;

class vrpd_analog : public flext_base
{
		FLEXT_HEADER(vrpd_analog,flext_base)
	 
	public:

		vrpd_analog(int argc, t_atom *argv);

	protected:

		void m_update();

	private:

		vrpn_Analog_Remote* m_analog;
		
		static void handleAnalog(void *userdata, vrpn_ANALOGCB t);
		void process(vrpn_ANALOGCB t);

		FLEXT_CALLBACK(m_update);  
	
};

FLEXT_NEW_V("vrpd_analog",vrpd_analog)


vrpd_analog::vrpd_analog(int argc, t_atom *argv) {
	//bang for new values
	AddInAnything();
	//sensor id + value
	AddOutList();

	FLEXT_ADDMETHOD(0,m_update);

	if(argc != 0) { // check for arg count
		post("vrpd_analog, a client for the VRPN protocol, by Florent Berthaut, SCRIME,  Université de Bordeaux. ");
		m_analog = new vrpn_Analog_Remote(GetString(argv[0]));
		m_analog->register_change_handler(this, handleAnalog);
	}
	else { 
		post("%s - no arguments given usage: vrpd_analog analogName@host", thisName());
		InitProblem();
	}

}

void vrpd_analog::m_update() {
	m_analog->mainloop();
}

void vrpd_analog::process(vrpn_ANALOGCB a) {
    if(a.num_channel>0) {
        AtomList list(a.num_channel);
        //fill number channels
        SetFloat(list[0],a.num_channel);
        //fill channels
        for(unsigned int c=0;c<a.num_channel-1;++c) {
            SetFloat(list[c+1],a.channel[c]);
        }

        ToOutList(0, a.num_channel, list.Atoms());
    }
}

void vrpd_analog::handleAnalog(void *userdata, vrpn_ANALOGCB a) {
	vrpd_analog* vrpd = (vrpd_analog*)userdata;
	vrpd->process(a);
}



