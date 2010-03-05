#include "maligner.hxx"

using namespace std;
using namespace Py;

MAligner::MAligner(){
    _mcore = new MAlignerCore();
}

MAligner::~MAligner(){
    delete _mcore;
}

Object MAligner::addEntry( const Tuple &args ){
    args.verify_length(2);
    String name = args[0];
    String sequence = args[1];
    
    _mcore->addEntry(name, sequence);
    return args[0];
}

Object MAligner::align( const Tuple &args ){
    args.verify_length(1);
    String sequence = args[0];
    pair<string, pair<string,string> > res = _mcore->bestAlign(sequence);
    string name = res.first;
    pair<string, string> trace = res.second;
    Tuple t(3);
    t[0] = name;
    t[1] = trace.first;
    t[2] = trace.second;
    return t;
}

/*
Object MAligner::alignWith( const Tuple &args ){
    args.verify_length(2);
    String sequence = args[0];
    List   refs = args[1];

    // dummy return
    return String("");
}
*/

Object MAligner::getattr( const char *name ){
    return getattr_methods(name);    
}

Object MAligner::repr(){
    return Py::String("Multiple Alignment Object");
}

void MAligner::init_type(){
    behaviors().name("MAligner");
    behaviors().doc("MAligner objects: nil");
    behaviors().supportGetattr();
    behaviors().supportRepr();

    add_varargs_method("addEntry",  &MAligner::addEntry, "addEntry(name, sequence): add an aligner entry");
    add_varargs_method("align",     &MAligner::align, "align(sequence): align a sequence against the reference");
    add_varargs_method("reference_count", &MAligner::reference_count);
}

class maligner_module : public Py::ExtensionModule<maligner_module>
{
public:
    maligner_module()
    : Py::ExtensionModule<maligner_module>( "maligner" ) // this must be name of the file on disk e.g. simple.so or simple.pyd
    {
        MAligner::init_type();
        add_varargs_method("MAligner",&maligner_module::new_maligner,"MAligner()");
        initialize( "documentation for the simple module" );
    }

    virtual ~maligner_module()
    {}

private:
    Object new_maligner(const Py::Tuple& args){
        return asObject(new MAligner());
    }

};

extern "C" void initmaligner()
{
#if defined(PY_WIN32_DELAYLOAD_PYTHON_DLL)
    Py::InitialisePythonIndirectPy::Interface();
#endif
    static maligner_module* maligner = new maligner_module;
}
