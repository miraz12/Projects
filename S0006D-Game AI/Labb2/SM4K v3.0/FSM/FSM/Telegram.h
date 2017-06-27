#pragma once
///Uppbyggnad av ett meddelande
struct Telegram
{

	int Sender;

	int Receiver;

	int Msg;

	double Time;

	void* ExtraInfo;

	Telegram() :Time(-1),
				Sender(-1),
				Receiver(-1),
				Msg(-1)
	{}

	Telegram(double time, int sender, int reciver, int msg, void* info = 0) :
		Time(time),
		Sender(sender),
		Receiver(reciver),
		Msg(msg),
		ExtraInfo(info)
	{}
	
	const float minDelay = 0.25f;

// 	inline bool operator==(const Telegram& t2)
// 	{
// 		const Telegram& t1 = *this;
// 
// 		return
// 	}

};