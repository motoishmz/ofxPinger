#pragma once
#include "ofxOscSender.h"
#include "ofxXmlSettings.h"

class ofxPinger
{
	
	const string filepath = "pinger.xml";
	
public:
	
	typedef struct
	{
		ofxOscSender sender;
		string host;
		int port;
		
		void setup(string h, int p)
		{
			host = h;
			port = p;
			sender.setup(host, port);
		}
		
		void bang(string address)
		{
			ofxOscMessage m;
			m.setAddress(address);
			m.addIntArg(1);
			sender.sendMessage(m);
		}
		
	} destination;
	
	vector<destination*> dests;
	
	
	// ---
	
	
	ofxPinger()
	: last_timestamp(0.0f)
	{
		
		ofxXmlSettings XML;
		XML.load(filepath);
		
		//! global settings
		ping_interval_ms = XML.getValue("settings:ping_interval_ms", 1000);
		ping_address = XML.getValue("settings:ping_address", "/ofxPinger");
		
		
		//! destinations
		XML.pushTag("destinations");
		{
			const int num_dests = XML.getNumTags("dest");
			
			for (int i=0; i<num_dests; i++)
			{
				const string host = XML.getValue("dest:host", "localhost", i);
				const int port = XML.getValue("dest:port", 10000, i);
				
				destination *d = new destination();
				d->setup(host, port);
				dests.push_back(d);
			}
		}
		XML.popTag();
		
		
		//! event hooks
		ofAddListener(ofEvents().setup, this, &ofxPinger::setup);
		ofAddListener(ofEvents().update, this, &ofxPinger::update);
		ofAddListener(ofEvents().exit, this, &ofxPinger::exit);
	}
	
	inline float getPingInterval() const { return ping_interval_ms; }
	inline string getPingAddress() const { return ping_address; }
	inline string getFilePath() const { return filepath; }
	
	
protected:
	
	float last_timestamp;
	float ping_interval_ms;
	string ping_address;
	
	bool shouldSendMessage() const
	{
		return (ofGetElapsedTimeMillis() - last_timestamp) > ping_interval_ms;
	}
	
	void setup(ofEventArgs &e)
	{
		for (auto d: dests)
			d->bang(ping_address+"/loadbang");
	}
	
	void update(ofEventArgs &e)
	{
		if (shouldSendMessage())
		{
			for (auto d: dests)
				d->bang(ping_address);
			
			last_timestamp = ofGetElapsedTimeMillis();
		}
	}
	
	void exit(ofEventArgs &e)
	{
		for (auto d: dests)
			delete d;
	}
};