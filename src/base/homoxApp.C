#include "homoxApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
homoxApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

homoxApp::homoxApp(const InputParameters & parameters) : MooseApp(parameters)
{
  homoxApp::registerAll(_factory, _action_factory, _syntax);
}

homoxApp::~homoxApp() {}

void
homoxApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAllObjects<homoxApp>(f, af, syntax);
  Registry::registerObjectsTo(f, {"homoxApp"});
  Registry::registerActionsTo(af, {"homoxApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
homoxApp::registerApps()
{
  registerApp(homoxApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
homoxApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  homoxApp::registerAll(f, af, s);
}
extern "C" void
homoxApp__registerApps()
{
  homoxApp::registerApps();
}
