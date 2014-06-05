#  ･゜･*:.｡..:*･'｡. .｡.:*･゜･*

ofxPinger is a simple wrapper of ofxOscSender which sends a periodic osc message for telling other apps that OF is working.

![image](https://raw.githubusercontent.com/motoishmz/ofxPinger/master/ofxaddons_thumbnail.png)

## Usage
1. Open `pinger.xml` in your data directory.
2. In \<destinations\> directive, you can define the destination(s) as \<dest\>.
3. In \<settings\> directive, you can set the ping interval time (ms) and the address of osc message.

## Dependencies
- ofxOsc
- ofxXmlSettings