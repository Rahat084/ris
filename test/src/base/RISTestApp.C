//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "RISTestApp.h"
#include "RISApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
RISTestApp::validParams()
{
  InputParameters params = RISApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

RISTestApp::RISTestApp(const InputParameters & parameters) : MooseApp(parameters)
{
  RISTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

RISTestApp::~RISTestApp() {}

void
RISTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  RISApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"RISTestApp"});
    Registry::registerActionsTo(af, {"RISTestApp"});
  }
}

void
RISTestApp::registerApps()
{
  registerApp(RISApp);
  registerApp(RISTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
RISTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  RISTestApp::registerAll(f, af, s);
}
extern "C" void
RISTestApp__registerApps()
{
  RISTestApp::registerApps();
}
