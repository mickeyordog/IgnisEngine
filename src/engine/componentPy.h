#pragma once

// #include "pythonEngine.h"
#include "component.h"

class ComponentPy : public Component {
public:
    ComponentPy() = default;
    virtual ~ComponentPy() = default;
    virtual void start() override = 0;
    virtual void update(float dt) override = 0;

    virtual bool isPy() override { return true; }
    virtual std::vector<FieldDescription>& getFields() override { return fields; };

private:
    std::vector<FieldDescription> fields = { };
    // PyObject* pyStartFunc; // not sure if need start too
    // PyObject* pyUpdateFunc; // TODO: make sure to call Py_DECREF on these
    // TODO: need to pass in something to constructor so it can get ref to functions
    // maybe python engine can be global static so I can just call invoke function from anywhere
};