#include "ns3module.h"
#include "ns3/ref-count-base.h"
#include <unistd.h>


namespace {


class PythonEventImpl : public ns3::EventImpl
{
private:
  PyObject *m_callback;
  PyObject *m_args;

public:

  PythonEventImpl (PyObject *callback, PyObject *args, PyObject *py_context=NULL)
  {
    m_callback = callback;
    Py_INCREF (m_callback);
    
    if (py_context == NULL)
      {
        m_args = args;
        Py_INCREF (m_args);
      }
    else 
      {
        Py_ssize_t arglen = PyTuple_GET_SIZE (args);
        m_args = PyTuple_New (arglen + 1);
        PyTuple_SET_ITEM (m_args, 0, py_context);
        Py_INCREF (py_context);
        for (Py_ssize_t i = 0; i < arglen; ++i) 
          {
            PyObject *arg = PyTuple_GET_ITEM (args, i);
            Py_INCREF (arg);
            PyTuple_SET_ITEM (m_args, 1 + i, arg);
          }
      }
  }

  virtual ~PythonEventImpl ()
  {
    PyGILState_STATE __py_gil_state = PyGILState_Ensure ();

    Py_DECREF (m_callback);
    Py_DECREF (m_args);

    PyGILState_Release (__py_gil_state);
  }
  virtual void Notify ()
  {
    PyGILState_STATE __py_gil_state = PyGILState_Ensure ();

    PyObject *retval = PyObject_CallObject (m_callback, m_args);
    if (retval) {
        if (retval != Py_None) {
            PyErr_SetString (PyExc_TypeError, "event callback should return None");
            PyErr_Print ();
          }
        Py_DECREF (retval);
      } else {
        PyErr_Print ();
      }

    PyGILState_Release (__py_gil_state);
  }
};

} // closes: namespace {

PyObject *
_wrap_Simulator_Schedule (PyNs3Simulator *PYBINDGEN_UNUSED (dummy), PyObject *args, PyObject *kwargs,
                          PyObject **return_exception)
{
  PyObject *exc_type, *traceback;
  PyObject *py_time;
  PyObject *py_callback;
  PyObject *user_args;
  ns3::Ptr<PythonEventImpl> py_event_impl;
  PyNs3EventId *py_EventId;

  if (kwargs && PyObject_Length (kwargs) > 0) {
      PyErr_SetString (PyExc_TypeError, "keyword arguments not supported");
      goto error;
    }

  if (PyTuple_GET_SIZE (args) < 2) {
      PyErr_SetString (PyExc_TypeError, "ns3.Simulator.Schedule needs at least 2 arguments");
      goto error;
    }
  py_time = PyTuple_GET_ITEM (args, 0);
  py_callback = PyTuple_GET_ITEM (args, 1);

  if (!PyObject_IsInstance (py_time, (PyObject*) &PyNs3Time_Type)) {
      PyErr_SetString (PyExc_TypeError, "Parameter 1 should be a ns3.Time instance");
      goto error;
    }
  if (!PyCallable_Check (py_callback)) {
      PyErr_SetString (PyExc_TypeError, "Parameter 2 should be callable");
      goto error;
    }
  user_args = PyTuple_GetSlice (args, 2, PyTuple_GET_SIZE (args));
  py_event_impl = ns3::Create<PythonEventImpl>(py_callback, user_args);
  Py_DECREF (user_args);

  py_EventId = PyObject_New (PyNs3EventId, &PyNs3EventId_Type);
  py_EventId->obj = new ns3::EventId (
      ns3::Simulator::Schedule (*((PyNs3Time *) py_time)->obj, py_event_impl));
  py_EventId->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
  return (PyObject *) py_EventId;

error:
  PyErr_Fetch (&exc_type, return_exception, &traceback);
  Py_XDECREF (exc_type);
  Py_XDECREF (traceback);
  return NULL;
}


PyObject *
_wrap_Simulator_ScheduleNow (PyNs3Simulator *PYBINDGEN_UNUSED (dummy), PyObject *args, PyObject *kwargs,
                             PyObject **return_exception)
{
  PyObject *exc_type, *traceback;
  PyObject *py_callback;
  PyObject *user_args;
  ns3::Ptr<PythonEventImpl> py_event_impl;
  PyNs3EventId *py_EventId;

  if (kwargs && PyObject_Length (kwargs) > 0) {
      PyErr_SetString (PyExc_TypeError, "keyword arguments not supported");
      goto error;
    }

  if (PyTuple_GET_SIZE (args) < 1) {
      PyErr_SetString (PyExc_TypeError, "ns3.Simulator.Schedule needs at least 1 argument");
      goto error;
    }
  py_callback = PyTuple_GET_ITEM (args, 0);

  if (!PyCallable_Check (py_callback)) {
      PyErr_SetString (PyExc_TypeError, "Parameter 1 should be callable");
      goto error;
    }
  user_args = PyTuple_GetSlice (args, 1, PyTuple_GET_SIZE (args));
  py_event_impl = ns3::Create<PythonEventImpl>(py_callback, user_args);
  Py_DECREF (user_args);

  py_EventId = PyObject_New (PyNs3EventId, &PyNs3EventId_Type);
  py_EventId->obj = new ns3::EventId (ns3::Simulator::ScheduleNow (py_event_impl));
  py_EventId->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
  return (PyObject *) py_EventId;

error:
  PyErr_Fetch (&exc_type, return_exception, &traceback);
  Py_XDECREF (exc_type);
  Py_XDECREF (traceback);
  return NULL;
}

PyObject *
_wrap_Simulator_ScheduleWithContext (PyNs3Simulator *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs,
                                     PyObject **return_exception)
{
    PyObject *exc_type, *traceback;
    PyObject *py_obj_context;
    uint32_t context;
    PyObject *py_time;
    PyObject *py_callback;
    PyObject *user_args;
    PythonEventImpl *py_event_impl;

    if (kwargs && PyObject_Length(kwargs) > 0) {
        PyErr_SetString(PyExc_TypeError, "keyword arguments not supported");
        goto error;
    }

    if (PyTuple_GET_SIZE(args) < 3 ) {
        PyErr_SetString(PyExc_TypeError, "ns3.Simulator.ScheduleWithContext needs at least 3 arguments");
        goto error;
    }

    py_obj_context = PyTuple_GET_ITEM(args, 0);
    py_time = PyTuple_GET_ITEM(args, 1);
    py_callback = PyTuple_GET_ITEM(args, 2);

    context = PyInt_AsUnsignedLongMask(py_obj_context);
    if (PyErr_Occurred()) {
        goto error;
    }
    if (!PyObject_IsInstance(py_time, (PyObject*) &PyNs3Time_Type)) {
        PyErr_SetString(PyExc_TypeError, "Parameter 2 should be a ns3.Time instance");
        goto error;
    }
    if (!PyCallable_Check(py_callback)) {
        PyErr_SetString(PyExc_TypeError, "Parameter 3 should be callable");
        goto error;
    }

    user_args = PyTuple_GetSlice(args, 3, PyTuple_GET_SIZE(args));
    py_event_impl = new PythonEventImpl (py_callback, user_args, py_obj_context);
    Py_DECREF(user_args);

    ns3::Simulator::ScheduleWithContext(context, *((PyNs3Time *) py_time)->obj, py_event_impl);

    Py_INCREF(Py_None);
    return Py_None;

error:
    PyErr_Fetch(&exc_type, return_exception, &traceback);
    Py_XDECREF(exc_type);
    Py_XDECREF(traceback);
    return NULL;
}

PyObject *
_wrap_Simulator_ScheduleDestroy (PyNs3Simulator *PYBINDGEN_UNUSED (dummy), PyObject *args, PyObject *kwargs,
                                 PyObject **return_exception)
{
  PyObject *exc_type, *traceback;
  PyObject *py_callback;
  PyObject *user_args;
  ns3::Ptr<PythonEventImpl> py_event_impl;
  PyNs3EventId *py_EventId;

  if (kwargs && PyObject_Length (kwargs) > 0) {
      PyErr_SetString (PyExc_TypeError, "keyword arguments not supported");
      goto error;
    }

  if (PyTuple_GET_SIZE (args) < 1) {
      PyErr_SetString (PyExc_TypeError, "ns3.Simulator.Schedule needs at least 1 argument");
      goto error;
    }
  py_callback = PyTuple_GET_ITEM (args, 0);

  if (!PyCallable_Check (py_callback)) {
      PyErr_SetString (PyExc_TypeError, "Parameter 2 should be callable");
      goto error;
    }
  user_args = PyTuple_GetSlice (args, 1, PyTuple_GET_SIZE (args));
  py_event_impl = ns3::Create<PythonEventImpl>(py_callback, user_args);
  Py_DECREF (user_args);

  py_EventId = PyObject_New (PyNs3EventId, &PyNs3EventId_Type);
  py_EventId->obj = new ns3::EventId (ns3::Simulator::ScheduleDestroy (py_event_impl));
  py_EventId->flags = PYBINDGEN_WRAPPER_FLAG_NONE;
  return (PyObject *) py_EventId;

error:
  PyErr_Fetch (&exc_type, return_exception, &traceback);
  Py_XDECREF (exc_type);
  Py_XDECREF (traceback);
  return NULL;
}


PyObject *
_wrap_TypeId_LookupByNameFailSafe (PyNs3TypeId *PYBINDGEN_UNUSED (dummy), PyObject *args, PyObject *kwargs,
                                   PyObject **return_exception)
{
  bool ok;
  const char *name;
  Py_ssize_t name_len;
  ns3::TypeId tid;
  PyNs3TypeId *py_tid;
  const char *keywords[] = { "name", NULL};

  if (!PyArg_ParseTupleAndKeywords (args, kwargs, (char *) "s#", (char **) keywords, &name, &name_len)) {
      PyObject *exc_type, *traceback;
      PyErr_Fetch (&exc_type, return_exception, &traceback);
      Py_XDECREF (exc_type);
      Py_XDECREF (traceback);
      return NULL;
    }
  ok = ns3::TypeId::LookupByNameFailSafe (std::string (name, name_len), &tid);
  if (!ok)
    {
      PyErr_Format (PyExc_KeyError, "The ns3 type with name `%s' is not registered", name);
      return NULL;
    }

  py_tid = PyObject_New (PyNs3TypeId, &PyNs3TypeId_Type);
  py_tid->obj = new ns3::TypeId (tid);
  PyNs3TypeId_wrapper_registry[(void *) py_tid->obj] = (PyObject *) py_tid;

  return (PyObject *) py_tid;
}


class CommandLinePythonValueSetter : public ns3::RefCountBase
{
  PyObject *m_namespace;
  std::string m_variable;
public:
  CommandLinePythonValueSetter (PyObject *ns, std::string const &variable) {
    Py_INCREF (ns);
    m_namespace = ns;
    m_variable = variable;
  }
  bool Parse (std::string value) {
    PyObject *pyvalue = PyString_FromStringAndSize (value.data (), value.size ());
    PyObject_SetAttrString (m_namespace, (char *) m_variable.c_str (), pyvalue);
    if (PyErr_Occurred ()) {
        PyErr_Print ();
        return false;
      }
    return true;
  }
  virtual ~CommandLinePythonValueSetter () {
    Py_DECREF (m_namespace);
    m_namespace = NULL;
  }

};

PyObject *
_wrap_CommandLine_AddValue (PyNs3CommandLine *self, PyObject *args, PyObject *kwargs,
                            PyObject **return_exception)
{
  const char *name, *help, *variable = NULL;
  PyObject *py_namespace = NULL;
  const char *keywords[] = { "name", "help", "variable", "namespace", NULL};

  if (!PyArg_ParseTupleAndKeywords (args, kwargs, (char *) "ss|sO", (char **) keywords, &name, &help, &variable, &py_namespace)) {
      PyObject *exc_type, *traceback;
      PyErr_Fetch (&exc_type, return_exception, &traceback);
      Py_XDECREF (exc_type);
      Py_XDECREF (traceback);
      return NULL;
    }

  if (variable == NULL) {
      variable = name;
    }
  if (py_namespace == NULL) {
      py_namespace = (PyObject *) self;
    }

  ns3::Ptr<CommandLinePythonValueSetter> setter = ns3::Create<CommandLinePythonValueSetter> (py_namespace, variable);
  self->obj->AddValue (name, help, ns3::MakeCallback (&CommandLinePythonValueSetter::Parse, setter));

  Py_INCREF (Py_None);
  return Py_None;
}

class PythonSimulator
{
public:
  PythonSimulator();
  void Run(void);
  bool IsFailed(void) const;

private:
  void DoCheckSignals(void);
  void DoRun(void);
  volatile bool m_stopped;
  bool m_failed;
  volatile bool m_isCheckPending;
  ns3::Ptr<ns3::SystemThread> m_thread;
  PyThreadState *m_py_thread_state;
};

PythonSimulator::PythonSimulator()
  : m_stopped(false),
    m_failed(false),
    m_isCheckPending(false)
{
  m_thread = ns3::Create<ns3::SystemThread>(ns3::MakeCallback(&PythonSimulator::DoRun, this));
  m_py_thread_state = NULL;
}

void
PythonSimulator::Run(void)
{
  m_failed = false;
  m_stopped = false;
  m_isCheckPending = false;
  m_thread->Start();

  Py_BEGIN_ALLOW_THREADS;
  
  ns3::Simulator::Run ();

  Py_END_ALLOW_THREADS;

  m_stopped = true;
  m_thread->Join();
}

bool 
PythonSimulator::IsFailed(void) const
{
  return m_failed;
}

void
PythonSimulator::DoCheckSignals(void)
{
  PyGILState_STATE __py_gil_state = PyGILState_Ensure ();

  PyErr_CheckSignals ();
  if (PyErr_Occurred ())
    {
      m_failed = true;
      ns3::Simulator::Stop();
    }

  PyGILState_Release (__py_gil_state);

  m_isCheckPending = false;
}

void 
PythonSimulator::DoRun(void)
{
  while (!m_stopped)
    {
      if (!m_isCheckPending)
        {
          m_isCheckPending = true;
          ns3::Simulator::ScheduleWithContext(0xffffffff, ns3::Seconds(0.0),
                                              &PythonSimulator::DoCheckSignals, this);
        }
      usleep(200000);
    }
}

PyObject *
_wrap_Simulator_Run (PyNs3Simulator *PYBINDGEN_UNUSED (dummy), PyObject *args, PyObject *kwargs,
                     PyObject **return_exception)
{
  PythonSimulator simulator;
  simulator.Run();
  if (simulator.IsFailed())
    {
      return NULL;
    }
  Py_INCREF (Py_None);
  return Py_None;
}
