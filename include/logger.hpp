#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>

namespace dh {

//should be suffienciently large as to handle even the largest logging and error messages
#define BUFFER_SIZE 4096

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Logger
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Singleton logger for any message that should be logged for debugging, or
 *    printed to stdout.
 *
 * Takes: (via constructor)
 * -> log_path
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
class Logger {
private:
	//buffer to store messages before writing
	std::array<char, BUFFER_SIZE> buffer;

  //index of first unused buffer char
	std::size_t buffer_tail = 0; 
  
  //the actual log file
	std::ofstream	log_file;

  bool setup   = false;
  bool debug   = false;
  bool quiet   = false;
  bool verbose = false;

  //CONSTRUCTOR & DESTRUCTOR
	Logger() {}
	~Logger() {
		//flush buffer one last time in teardown.
		(void) flushBuffer();
		log_file.close();
	}

  /*
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * flushBuffer
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Description:
   * -> Appends buffer contents to open file.
   *
   * Returns:
   * -> On success:
   *    EXIT_SUCCESS
   * -> On failure:
   *    EXIT_FAILURE
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   */
	int flushBuffer();

  /*
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * appendToLog
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Description:
   * -> Appends a message to the buffer. Automatically flushes buffer to file
   *    when full. Errors if initialize() was not called already.
   *
   * Takes:
   * -> message:
   *    The log message to write. Should be <= 4096 characters.
   *
   * Returns:
   * -> On success:
   *    EXIT_SUCCESS
   * -> On failure:
   *    EXIT_FAILURE
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   */
	int appendToLog(std::string const& message);  

public:
  /*
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * initialize
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Description:
   * -> Opens log file, prepares buffer.
   *
   * Returns:
   * -> On success:
   *    EXIT_SUCCESS
   * -> On failure:
   *    EXIT_FAILURE
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   */
  int initialize(std::string const& path, 
                 bool               debug, 
                 bool               quiet, 
                 bool               verbose);

  /*
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * err/warn/status/log
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * -> Formats the provided message based on type, and appends it to the log
   *    if appropriate. Certain message types will also be printed to the
   *    console depending on the situation:
   *    -> Printed always:
   *       - err()
   *    -> Printed unless quiet flag set:
   *       - warn()
   *       - status()
   *    -> Printed only if verbose flag set:
   *       - log()
   *
   *    If debug mode is set, all messages will be recorded in the log.
   *
   * Takes:
   * -> message:
   *    The message to format and log/print.
   *
   * Returns:
   * -> On success:
   *    EXIT_SUCCESS
   * -> On failure:
   *    EXIT_FAILURE
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   */
  int log   (std::string const& message);
  int err   (std::string const& message);
  int warn  (std::string const& message);
  int status(std::string const& message);

  //GETTER
  static Logger& get() {
    static Logger instance;
    return instance;
  }

  //COPY & MOVE
  Logger(Logger const&)         = delete;
  void operator=(Logger const&) = delete;

};





}
