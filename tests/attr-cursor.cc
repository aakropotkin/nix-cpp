/* ========================================================================== *
 *
 *
 *
 * -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include <nix/shared.hh>
#include <nix/eval.hh>
#include <nix/eval-cache.hh>
#include <nix/flake/flake.hh>
#include <nix/store-api.hh>
#include "attrs-iter.hh"


/* -------------------------------------------------------------------------- */

  int
main( int argc, char * argv[], char ** envp )
{
  /* Boilerplate */
  nix::initNix();
  nix::initGC();
  nix::evalSettings.pureEval     = true; 
  nix::evalSettings.useEvalCache = true;
  nix::ref<nix::Store>     store = nix::ref<nix::Store>( nix::openStore() );
  nix::ref<nix::EvalState> state = nix::make_ref<nix::EvalState>(
    std::list<std::string> {}, store, store
  );
  state->repair = nix::NoRepair;
  std::shared_ptr<nix::flake::LockedFlake> flake =
    std::make_shared<nix::flake::LockedFlake>(
      nix::flake::lockFlake(
        * state
      , nix::parseFlakeRef( "github:NixOS/nixpkgs" )
      , (nix::flake::LockFlags) {
            .updateLockFile = false
          , .writeLockFile = false
        }
      )
    );
  /* End boilerplate */


/* -------------------------------------------------------------------------- */

  /* Use our iterator. */

  std::list<std::string> path = {
    "legacyPackages", "x86_64-linux", "hello", "meta"
  };
  /* Try printing all key/values in `hello.meta'. */
  nixcpp::util::AttrSetIterClosure helloMeta( state, flake, path );
  for ( const auto & [path, cursor] : helloMeta )
    {
      try
        {
          std::string str = cursor->getString();
          std::cout << path.back() << ": " << str << std::endl;
        }
      catch( ... ) {}
    }


/* -------------------------------------------------------------------------- */

  return EXIT_SUCCESS;

}  /* End function `main' */


/* -------------------------------------------------------------------------- *
 *
 *
 *
 * ========================================================================== */
