#ifndef __ERROR_REPORTER_H__
#define __ERROR_REPORTER_H__

#include "config_host.h"

enum Error : host::uint_t {
    OK = 0,
    INVALID_STATE,
};

class ReportError {
public:
    Error getError() const { return _error; }
    bool hasError() const { return !(_error == OK); }
protected:
    Error setError(Error error) {
        _error = error;
        return error;
    }
    Error setError(const ReportError &errorReporter) {
        return setError(errorReporter.getError());
    }
    void resetError() {
        _error = INVALID_STATE;
    }
private:
    Error _error;
};

#endif // __ERROR_REPORTER_H__

