// include flext header
#include <flext/flext.h>
#include <iostream>

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 401)
#error You need at least flext version 0.4.1
#endif

#include <vrpn_Button.h>

using namespace std;

class vrpd_button : public flext_base {
		FLEXT_HEADER(vrpd_button,flext_base)
	 
	public:
		vrpd_button(int argc, t_atom *argv);

	protected:
		void m_update();

	private:
		vrpn_Button_Remote* m_button;
		
		static void handleButton(void *userdata, vrpn_BUTTONCB t);
		void process(vrpn_BUTTONCB t);

		FLEXT_CALLBACK(m_update);  
};

FLEXT_NEW_V("vrpd_button",vrpd_button)

vrpd_button::vrpd_button(int argc, t_atom *argv) {
	//bang for new values
	AddInAnything();
	//sensor id + button value
	AddOutList();

	FLEXT_ADDMETHOD(0,m_update);

	if(argc != 0) { // check for arg count
		post("vrpd_button, a client for the VRPN protocol, by Florent Berthaut (hitmuri.net), SCRIME,  Université de Bordeaux. ");
		m_button = new vrpn_Button_Remote(GetString(argv[0]));
		m_button->register_change_handler(this, handleButton);
	}
	else { 
		post("%s - no arguments given usage: vrpd_button buttonName@host", thisName());
		InitProblem();
	}

}

void vrpd_button::m_update() {
	m_button->mainloop();
}

void vrpd_button::process(vrpn_BUTTONCB b) {
	AtomList list(2);
	//fill id
	SetFloat(list[0],b.button);
    //fill button state
	SetFloat(list[1],b.state);

	ToOutList(0, 2, list.Atoms());
}

void vrpd_button::handleButton(void *userdata, vrpn_BUTTONCB t) {
	vrpd_button* vrpd = (vrpd_button*)userdata;
	vrpd->process(t);
}

