/**
 * @file logstream.h	provide a class definition for logging output
 *
 * a logstream can be used in much the same way as standard C++ output
 * streams; however, one can define different logging facilities and
 * severities, and one can select how severe an event being logged
 * must be to actually show up in the output. this way, one can separate
 * debugging output from more severe warnings and turn off debugging
 * output during normal program runs. see below for examples of how to
 * use/define a logstream.
 *
 * Dec 2007 - Jan 2008	M. Schiller
 * 			initial development, debugging
 * Feb 10 2008		M. Schiller
 * 			first publically available revision
 * Oct 02 2008		M. Schiller
 * 			fixed type conversion bug, handle newline in
 * 			printed strings better
 * Dec 24 2013 P. Seyfert
 *      import into ZooReaders
 *
 * @author Manuel Schiller
 * @date Dec 2007
 */
#ifndef _LOGSTREAM_H
#define _LOGSTREAM_H

#include <map>
#include <memory>
#include <string>
#include <ostream>
#include <sstream>

/// namespace to hold logstream class
namespace logstreams {

  enum levels {
    DEBUG=0,
    WARNING=1,
    INFO=2,
    ERROR=3,
    FATAL=4,
    ALWAYS=5
  };
  /// class to handle logging to an ostream
  /** the idea behind this class is that logging to some kind of ostream
   * should be convenient:
   * - it should be possible to switch logging on and off
   * - the log file should contain which part of the software is logging
   * - should behave more or less like cout/cerr from a user's point of
   *   view
   */
  class logstream {
    public:
      /// get a logstream
      /**
       * get and release
       *
       * get a message stream based on ostream s, with a message level
       * mylevel and a given name
       * the reference to the logstream object can be used very much like
       * a C++ ostream object:
       *
       * 		logstream& mylog =
       * 			logstream::get(std::cout, 2, "MyLog");
       * 		mylog << "Hello, world!" << std::endl;
       *
       * should produce something similar to
       *
       * 		MyLog	Hello, world!
       *
       * on the standard output, provided that the global log level is
       * no higher than 2 (see below), i.e. a logstream will only
       * produce output when its log level is greater or equal the global
       * log level
       * 
       * don't use get excessively, rather keep your reference to the
       * logstream until you don't need it any longer, because the
       * get operation is not cheap
       * there are some pre-canned stream available for general use,
       * see below
       */
      static logstream& get(std::ostream& s, int mylevel,
          const std::string& name);

      /// clone this logstream and return a reference to the clone
      logstream& clone();

      /// once you don't need the logstream any longer, you can release it
      void release();

      /// make a log stream permanent
      /** make a log stream permanent, i.e. set a flag such that release
       * will become a no-op for that stream
       * this is useful to make sure that standard streams remain
       * available
       *
       * usage example:
       *
       * 	    logstream& logic =
       * 		logstream::get(std::cerr, 7, "LOGIC").makePermanent();
       */
      logstream& makePermanent();
      /// check if a logstream is permanent
      bool isPermanent() const;

      /// check if a logstream with given attributes is already registered
      /** check if a logstream with a given underlying stream, log level
       * and name does already exist
       */
      static bool isRegistered(std::ostream& stream, int logLevel,
          const std::string& name);

      /// reattach a log stream
      /** reattach to a different stream, log level and name; the stream
       * is still referred to using the old parameters
       *
       * the most useful application for this method is probably to make
       * the output of one of the standard streams go to a file...
       */
      logstream& reattach(std::ostream& newstream, int& newlevel,
          const std::string& newname);

      /// streaming operator to pass through manipulators like std::endl
      inline logstream& operator<< (std::ostream& (*par)(std::ostream&));
      /// streaming operator
      template<typename T> inline logstream& operator<< (const T& par);

      /// flush internal buffers
      /** if flushStream is true, the underlying stream is flushed
       * as well
       */
      void flush(bool flushStream = false);

      /// get underlying ostream
      std::ostream& getStream() const;
      /// get log level
      int getLogLevel() const;
      /// get the name of logstream
      const std::string& getName() const;

      /// provide access to the global log level
      static int logLevel();
      /// set the global log level and return its previous value
      static int setLogLevel(int level);

      /// get current width of logstream name field
      /** provide access to the current width of the field for the
       * logstream's name in the output
       */
      static unsigned nameWidth();
      /// set width of logstream name field
      /** set the maximum width of the field for the logstream's name
       * returns the previous value
       */
      static unsigned setMaxNameWidth(unsigned maxwidth);

    private:
      ////////////////////////////////////////////////////////////////
      // member variables
      ////////////////////////////////////////////////////////////////
      std::ostream* stream;	///< underlying ostream
      int myLogLevel;		///< loglevel of this logstream
      std::string myname;		///< name of this logstream
      bool bufdirty;		///< true if buffer holds pending data
      std::ostringstream buffer;	///< buffer
      unsigned nUsers;		///< number of users of this log stream

      ////////////////////////////////////////////////////////////////
      // hidden constructors/destructors/operators
      ////////////////////////////////////////////////////////////////
      // private constructors, destructors and assignment operators -
      // we do not want the user to create or destroy streams in an
      // uncontrolled way
      logstream(std::ostream* s, int mylevel, const std::string& name);
      logstream();
      logstream(const logstream&);
      logstream& operator=(const logstream&);
      ~logstream();

      ////////////////////////////////////////////////////////////////
      // static class members and methods
      ////////////////////////////////////////////////////////////////
      static int globalLogLevel;	///< global log level
      static unsigned namewidth;  ///< width of the name field

      /// keys in the mapping (stream, level, name) -> logstream
      class StreamMapKey {
        public:
          const std::ostream* stream;
          int level;
          const std::string name;
          StreamMapKey(const std::ostream* s, int l,
              const std::string& n) :
            stream(s), level(l), name(n)
        { }
      };
      /// comparison object for the keys
      class StreamMapCompare : public std::binary_function<
                               const StreamMapKey&, const StreamMapKey&, bool>
    {
      public:
        inline bool operator() (const StreamMapKey& a,
            const StreamMapKey& b)
        {
          if (a.stream < b.stream) return true;
          else if (b.stream < a.stream) return false;
          else {
            if (a.level < b.level) return true;
            else if (b.level < a.level) return false;
            else {
              if (a.name < b.name) return true;
              else return false;
            }
          }
        }
    };
      /// map type for (ostream, level, name) -> logstream*
      typedef std::map<StreamMapKey, logstream*, StreamMapCompare>
        StreamMap;
      /// map for (ostream, level, name) -> logstream*
      static std::auto_ptr<StreamMap> streams;

      /// cleanup method to be used at exit
      static void flushStreams();
      /// register a stream internally
      static void registerStream(logstream* log);
      /// unregister a stream internally
      static void unregisterStream(logstream* log, bool doDelete);

      /// deleter for map-style iterators containing pointers...
      template<typename K, typename T>
        struct deleter :
          public std::unary_function<const std::pair<K, T*>&, void> {
            void operator()(const std::pair<K,T*>& ptr)
            { delete ptr.second; }
          };
  };

  ////////////////////////////////////////////////////////////////////
  // return references to some pre-canned log streams
  ////////////////////////////////////////////////////////////////////
  // usage: e.g.
  //
  // 		info << "Some nice message..." << std::endl;
  //
  extern logstream& debug;	///< global debug stream
  extern logstream& info;	///< global info stream
  extern logstream& warn;	///< global warning stream
  extern logstream& error;	///< global error stream
  extern logstream& fatal;	///< global fatal stream
  extern logstream& always;	///< global always stream

  ////////////////////////////////////////////////////////////////////
  // implementation of operator<< methods
  ////////////////////////////////////////////////////////////////////
  // logstream's operator<< methods are (partly) templated and will
  // be called often, so they are in the header and inline
  template<typename T> inline logstream& logstream::operator<< (const T& par)
  {
    if (myLogLevel >= globalLogLevel) {
      // writing to the stream makes the buffer dirty
      bufdirty = true;
      buffer << par;
    }
    return *this;
  }

  inline logstream& logstream::operator<<(std::ostream&(*par)(std::ostream&))
  {
    if (myLogLevel >= globalLogLevel) {
      // writing to the stream makes the buffer dirty
      bufdirty = true;
      par(buffer);
      // get pointer to endl so we can flush buffers to the underlying
      // stream if par was endl
      std::ostream& (*endl)(std::ostream&) = std::endl;
      if (endl == par)
        flush();
    }
    return *this;
  }
}

#endif
// vim:sw=4:tw=78:ft=cpp
