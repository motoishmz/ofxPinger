#pragma once
#include "ofxOscSender.h"
#include "ofxXmlSettings.h"

class ofxPinger
{
	
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
	: last_ping_timestamp(0.0f)
	{
		ofAddListener(ofEvents().update, this, &ofxPinger::update);
	}
	
	virtual ~ofxPinger()
	{
		for (auto d: dests)
			delete d;
		
		ofRemoveListener(ofEvents().update, this, &ofxPinger::update);
		ofLogNotice("ofxPinger") << "stoped pinging, bye.";
	}
	
	void setup(string filepath = "pinger.xml")
	{
		setting_filepath = filepath;
		
		ofxXmlSettings XML;
		XML.load(setting_filepath);
		
		
		//! global settings
		ping_interval_ms = XML.getValue("settings:ping_interval_ms", 1000);
		ping_address = XML.getValue("settings:ping_address", "/ofxPinger");
		
		
		//! destinations
		stringstream report;
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
				
				report << "[destination " << i << "] " << endl;
				report << "host: " << host << endl;
				report << "port: " << port << endl;
				report << endl;
			}
			
			if (num_dests > 0)
				ofLogNotice("ofxPinger") << "started pinging to " << num_dests << " destinations." << endl << report.str();
			else
				ofLogError("ofxPinger") << "couldn't load destinations from " << setting_filepath;
		}
		XML.popTag();
		
		
		//! loadbang
		for (auto d: dests)
			d->bang(ping_address+"/loadbang");
	}
	
	inline float getPingInterval() const { return ping_interval_ms; }
	inline string getPingAddress() const { return ping_address; }
	inline string getFilePath() const { return setting_filepath; }
	
	
protected:
	
	float last_ping_timestamp;
	float ping_interval_ms;
	string ping_address;
	string setting_filepath;
	
	bool shouldSendMessage() const
	{
		return (ofGetElapsedTimeMillis() - last_ping_timestamp) > ping_interval_ms;
	}
	
	void update(ofEventArgs &e)
	{
		if (shouldSendMessage())
		{
			for (auto d: dests)
				d->bang(ping_address);
			
			last_ping_timestamp = ofGetElapsedTimeMillis();
		}
	}
};