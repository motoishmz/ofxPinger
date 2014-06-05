#include "ofMain.h"
#include "ofxPinger.h"

class ofApp : public ofBaseApp
{
	
	ofxPinger pinger;
	
public:
	
	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		ofBackground(0, 3, 30);
	}
	
	void update()
	{
		
	}
	
	void draw()
	{
		//! global settings
		stringstream settings;
		settings << "[Setting file]  " << pinger.getFilePath() << endl;
		settings << "[Ping address]  " << pinger.getPingAddress() << endl;
		settings << "[Ping interval] " << pinger.getPingInterval() << " ms";
		ofDrawBitmapString(settings.str(), 20, 20);
		
		
		//! destinations
		ofTranslate(0, 70);
		for (int i=0; i<pinger.dests.size(); i++)
		{
			stringstream report;
			report << "[destination " << i << "]" << endl;
			report << "--------" << endl;
			report << "host:" << pinger.dests[i]->host << endl;
			report << "port:" << pinger.dests[i]->port;
			ofDrawBitmapStringHighlight(report.str(), 20+150*i, 20, ofColor::limeGreen);
		}
	}
};


#pragma mark -
#pragma mark main
int main(){
	ofSetupOpenGL(1024, 800, OF_WINDOW);
	ofRunApp(new ofApp());
}
