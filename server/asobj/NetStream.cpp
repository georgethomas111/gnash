// NetStream.cpp:  ActionScript class for streaming audio/video, for Gnash.
// 
//   Copyright (C) 2005, 2006, 2007 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

/* $Id: NetStream.cpp,v 1.39 2007/05/04 15:21:00 strk Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "NetStream.h"

#include "log.h"
#ifdef SOUND_GST
# include "NetStreamGst.h"
#elif defined(USE_FFMPEG)
# include "NetStreamFfmpeg.h"
#endif
#include "fn_call.h"
#include "builtin_function.h"
#include "GnashException.h"
#include "NetConnection.h"
#include "action.h" // for call_method

#include "movie_root.h"

namespace gnash {
 
static as_value netstream_new(const fn_call& fn);
static as_value netstream_close(const fn_call& fn);
static as_value netstream_pause(const fn_call& fn);
static as_value netstream_play(const fn_call& fn);
static as_value netstream_seek(const fn_call& fn);
static as_value netstream_setbuffertime(const fn_call& fn);
static as_value netstream_time(const fn_call& fn);
static as_object* getNetStreamInterface();

NetStream::NetStream()
	:
	as_object(getNetStreamInterface()),
	_netCon(NULL),
	m_env(NULL),
	_lastStatus(invalidStatus)
{
}

static as_value
netstream_new(const fn_call& fn)
{

	boost::intrusive_ptr<NetStream> netstream_obj;
       
#ifdef SOUND_GST
	netstream_obj = new NetStreamGst();
#elif defined(USE_FFMPEG)
	netstream_obj = new NetStreamFfmpeg();
#else
	netstream_obj = new NetStream();
#endif


	if (fn.nargs > 0)
	{
		boost::intrusive_ptr<NetConnection> ns = boost::dynamic_pointer_cast<NetConnection>(fn.arg(0).to_object());
		if ( ns )
		{
			netstream_obj->setNetCon(ns);
			netstream_obj->setEnvironment(&fn.env());			
		}
		else
		{
			IF_VERBOSE_ASCODING_ERRORS(
				log_aserror(_("First argument "
					"to NetStream constructor "
					"doesn't cast to a NetConnection (%s)"),
					fn.arg(0).to_string().c_str());
			);
		}
	}
	return as_value(netstream_obj.get());

}

static as_value netstream_close(const fn_call& fn)
{
	boost::intrusive_ptr<NetStream> ns = ensureType<NetStream>(fn.this_ptr);
	ns->close();
	return as_value();
}

static as_value netstream_pause(const fn_call& fn)
{
	boost::intrusive_ptr<NetStream> ns = ensureType<NetStream>(fn.this_ptr);
	
	// mode: -1 ==> toogle, 0==> pause, 1==> play
	int mode = -1;
	if (fn.nargs > 0)
	{
		mode = fn.arg(0).to_bool() ? 0 : 1;
	}
	ns->pause(mode);	// toggle mode
	return as_value();
}

static as_value netstream_play(const fn_call& fn)
{
	boost::intrusive_ptr<NetStream> ns = ensureType<NetStream>(fn.this_ptr);

	if (fn.nargs < 1)
	{
		IF_VERBOSE_ASCODING_ERRORS(
		log_aserror(_("NetStream play needs args"));
		);
		return as_value();
	}

	if (ns->play(fn.arg(0).to_string().c_str()) != 0)
	{
		ns->close();
	};
	return as_value();
}

static as_value netstream_seek(const fn_call& fn) {
	boost::intrusive_ptr<NetStream> ns = ensureType<NetStream>(fn.this_ptr);

	double time = 0;
	if (fn.nargs > 0)
	{
		time = fn.arg(0).to_number();
	}
	ns->seek(time);

	return as_value();
}
static as_value netstream_setbuffertime(const fn_call& fn) {
	boost::intrusive_ptr<NetStream> ns = ensureType<NetStream>(fn.this_ptr);

	double time = 0;
	if (fn.nargs > 0)
	{
		time = fn.arg(0).to_number();
	}
	ns->setBufferTime(time);

	return as_value();
}

// Both a getter and a (do-nothing) setter for time
static as_value
netstream_time(const fn_call& fn)
{

	boost::intrusive_ptr<NetStream> ns = ensureType<NetStream>(fn.this_ptr);

	return as_value(static_cast<double>(ns->time()));
}

// Both a getter and a (do-nothing) setter for bytesLoaded
static as_value
netstream_bytesloaded(const fn_call& fn)
{

	boost::intrusive_ptr<NetStream> ns = ensureType<NetStream>(fn.this_ptr);

	return as_value(int(ns->bytesLoaded()));
}

// Both a getter and a (do-nothing) setter for bytesTotal
static as_value
netstream_bytestotal(const fn_call& fn)
{

	boost::intrusive_ptr<NetStream> ns = ensureType<NetStream>(fn.this_ptr);

	return as_value(int(ns->bytesTotal()));
}

void
attachNetStreamInterface(as_object& o)
{

	o.init_member("close", new builtin_function(netstream_close));
	o.init_member("pause", new builtin_function(netstream_pause));
	o.init_member("play", new builtin_function(netstream_play));
	o.init_member("seek", new builtin_function(netstream_seek));
	o.init_member("setBufferTime", new builtin_function(netstream_setbuffertime));


    // Properties

    boost::intrusive_ptr<builtin_function> gettersetter;

    gettersetter = new builtin_function(&netstream_time, NULL);
    o.init_readonly_property("time", *gettersetter);

    gettersetter = new builtin_function(&netstream_bytesloaded, NULL);
    o.init_readonly_property("bytesLoaded", *gettersetter);

    gettersetter = new builtin_function(&netstream_bytestotal, NULL);
    o.init_readonly_property("bytesTotal", *gettersetter);

}

static as_object*
getNetStreamInterface()
{

	static boost::intrusive_ptr<as_object> o;
	if ( o == NULL )
	{
		o = new as_object();
		attachNetStreamInterface(*o);
	}

	return o.get();
}

// extern (used by Global.cpp)
void netstream_class_init(as_object& global)
{

	// This is going to be the global NetStream "class"/"function"
	static boost::intrusive_ptr<builtin_function> cl;

	if ( cl == NULL )
	{
		cl=new builtin_function(&netstream_new, getNetStreamInterface());
		// replicate all interface to class, to be able to access
		// all methods as static functions
		attachNetStreamInterface(*cl);
		     
	}

	// Register _global.String
	global.init_member("NetStream", cl.get());

}

void
NetStream::processStatusNotifications()
{
	// Get an exclusive lock so any notification from loader thread will wait
	boost::mutex::scoped_lock lock(statusMutex);

	// No queued statuses to notify ...
	if ( _statusQueue.empty() ) return;

	// TODO: check for System.onStatus too ! use a private getStatusHandler() method for this.
	as_value status;
	if ( get_member("onStatus", &status) && status.is_function())
	{
		log_debug("Processing %d status notifications", _statusQueue.size());

		for (StatusQueue::iterator it=_statusQueue.begin(), itE=_statusQueue.end(); it!=itE; ++it)
		{
			StatusCode code = *it; 

			log_debug(" Invoking onStatus(%s)", getStatusCodeInfo(code).first);

			// TODO: optimize by reusing the same as_object ?
			boost::intrusive_ptr<as_object> o = getStatusObject(code);

			m_env->push_val(as_value(o.get()));
			call_method(status, m_env, this, 1, m_env->get_top_index() );
		}

	}

	_statusQueue.clear();

}

void
NetStream::setStatus(StatusCode status)
{
	// status unchanged
	if ( _lastStatus == status) return;

	// Get a lock to avoid messing with statuses while processing them
	boost::mutex::scoped_lock lock(statusMutex);

	_lastStatus = status;
	_statusQueue.push_back(status);
}

std::pair<const char*, const char*>
NetStream::getStatusCodeInfo(StatusCode code)
{
	switch (code)
	{
	
		case bufferEmpty:
			return make_pair("NetStream.Buffer.Empty", "status");

		case bufferFull:
			return make_pair("NetStream.Buffer.Full", "status");

		case bufferFlush:
			return make_pair("NetStream.Buffer.Flush", "status");

		case playStart:
			return make_pair("NetStream.Play.Start", "status");

		case playStop:
			return make_pair("NetStream.Play.Stop", "status");

		case seekNotify:
			return make_pair("NetStream.Seek.Notify", "status");

		case streamNotFound:
			return make_pair("NetStream.Play.StreamNotFound", "error");

		case invalidTime:
			return make_pair("NetStream.Seek.InvalidTime", "error");

		default:
			return make_pair("","");
	}
}

boost::intrusive_ptr<as_object>
NetStream::getStatusObject(StatusCode code)
{
	// code, level
	std::pair<const char*, const char*> info = getStatusCodeInfo(code);

	boost::intrusive_ptr<as_object> o = new as_object();
	o->init_member("code",  info.first,  1);
	o->init_member("level", info.second, as_prop_flags::dontDelete|as_prop_flags::dontEnum);

	return o;
}


} // end of gnash namespace
