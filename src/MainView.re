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
    let container =
      Style.[
        position(`Absolute),
        top(0),
        left(0),
        right(0),
        height(48),
        justifyContent(`Center),
        paddingLeft(12),
        backgroundColor(HNColors.orange),
      ];
  };
};

let%component make = () => {
  let%hook (currentView, setCurrentView) = Hooks.state(<Home />);

  <View style=Styles.root>
    <View style=Styles.Header.container>
      <Text text="ReveryHackerNews" fontSize=20. fontWeight=Font.Weight.Bold />
    </View>
    <View style=Styles.container> currentView </View>
  </View>;
};
