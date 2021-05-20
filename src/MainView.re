open Revery;
open Revery.UI;

module Styles = {
  let root =
    Style.[
      position(`Absolute),
      bottom(0),
      top(0),
      left(0),
      right(0),
      backgroundColor(HNColors.background),
    ];

  let container =
    Style.[top(48), bottom(0), left(0), right(0), position(`Absolute)];

  module Header = {
    let home = Style.[cursor(MouseCursors.pointer)];
    let container =
      Style.[
        position(`Absolute),
        top(0),
        left(0),
        right(0),
        height(48),
        flexDirection(`Row),
        alignItems(`Center),
        paddingLeft(12),
        backgroundColor(HNColors.orange),
      ];
  };
};

let%component make = () => {
  let%hook (currentView, setCurrentView) =
    Hooks.reducer(~initialState=None, (value, _) => value);

  let onCommentsClicked = id => setCurrentView(Some(<Comments id />));
  let onHomeClicked = _ => setCurrentView(Some(<Home onCommentsClicked />));

  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        setCurrentView(Some(<Home onCommentsClicked />));
        None;
      },
    );

  let currentView = currentView |> Option.value(~default=<View />);

  <View style=Styles.root>
    <View style=Styles.Header.container>
      <Text
        text="🏠"
        fontSize=18.
        onMouseUp=onHomeClicked
        style=Styles.Header.home
      />
      <Text text="ReveryHackerNews" fontSize=20. fontWeight=Font.Weight.Bold />
    </View>
    <View style=Styles.container> currentView </View>
  </View>;
};
