// include flext header
#include <flext/flext.h>
#include <iostream>

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 401)
#error You need at least flext version 0.4.1
#endif

#include <vrpn_Tracker.h>

using namespace std;

class vrpd_tracker : public flext_base
{
		FLEXT_HEADER(vrpd_tracker,flext_base)
	 
	public:

		vrpd_tracker(int argc, t_atom *argv);

	protected:

		void m_update();

	private:

		vrpn_Tracker_Remote* m_tracker;
		
		static void handleTracker(void *userdata, vrpn_TRACKERCB t);
		void process(vrpn_TRACKERCB t);

		FLEXT_CALLBACK(m_update);  
	
};

FLEXT_NEW_V("vrpd_tracker",vrpd_tracker)


vrpd_tracker::vrpd_tracker(int argc, t_atom *argv) {
	//bang for new values
	AddInAnything();
	//sensor id
	//list of 3 float for position vector 
	//and 4 for orientation quaternion
	AddOutList();

	FLEXT_ADDMETHOD(0,m_update);

	if(argc != 0) { // check for arg count
		post("vrpd_tracker, a client for the VRPN protocol, by Florent Berthaut, SCRIME,  Université de Bordeaux. ");
		m_tracker = new vrpn_Tracker_Remote(GetString(argv[0]));
		m_tracker->register_change_handler(this, handleTracker);
	}
	else { 
		post("%s - no arguments given usage: vrpd_tracker trackerName@host", thisName());
		InitProblem();
	}

}

void vrpd_tracker::m_update() {
	m_tracker->mainloop();
}

void vrpd_tracker::process(vrpn_TRACKERCB t)
{
	AtomList list(8);
	//fill id
	SetFloat(list[0],t.sensor);

	//fill position
	SetFloat(list[1],t.pos[0]);
	SetFloat(list[2],t.pos[1]);
	SetFloat(list[3],t.pos[2]);

	//fill orientation
	SetFloat(list[4],t.quat[0]);
	SetFloat(list[5],t.quat[1]);
	SetFloat(list[6],t.quat[2]);
	SetFloat(list[7],t.quat[3]);

	ToOutList(0, 8, list.Atoms());
}

void vrpd_tracker::handleTracker(void *userdata, vrpn_TRACKERCB t) {
	vrpd_tracker* vrpd = (vrpd_tracker*)userdata;
	vrpd->process(t);
}



