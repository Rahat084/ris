#include "RISApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
RISApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

RISApp::RISApp(const InputParameters & parameters) : MooseApp(parameters)
{
  RISApp::registerAll(_factory, _action_factory, _syntax);
}

RISApp::~RISApp() {}

void
RISApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAllObjects<RISApp>(f, af, syntax);
  Registry::registerObjectsTo(f, {"RISApp"});
  Registry::registerActionsTo(af, {"RISApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
RISApp::registerApps()
{
  registerApp(RISApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
RISApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  RISApp::registerAll(f, af, s);
}
extern "C" void
RISApp__registerApps()
{
  RISApp::registerApps();
}
