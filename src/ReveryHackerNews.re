open Revery;
open Revery.UI;

let init = app => {
  Revery.App.initConsole();
  Timber.App.enable(Timber.Reporter.console());
  Timber.App.setLevel(Timber.Level.perf);
  ReQuests.init();

  let tick = _ => {
    let _: bool = Luv.Loop.run(~mode=`NOWAIT, ());
    ();
  };

  let _: unit => unit =
    Tick.interval(~name="ReveryHackerNews event loop", tick, Time.zero);

  let (initialWidth, initialHeight) = (800, 600);

  let window =
    App.createWindow(
      ~createOptions=
        WindowCreateOptions.create(
          ~width=initialWidth,
          ~height=initialHeight,
          ~acceleration=`Auto,
          (),
        ),
      app,
      "ReveryHackerNews",
    );

  let _renderFunction = UI.start(window, <MainView />);
  ();
};

Printexc.record_backtrace(true);
App.start(init);
