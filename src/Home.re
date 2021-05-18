open Revery;
open Revery.UI;
open Revery.UI.Transform;
open ReQuests;

module Styles = {
  let container = Style.[overflow(`Scroll), flexGrow(1)];
  let list = (~offset) => {
    let theTransform = [TranslateY(offset)];
    Style.[transform(theTransform)];
  };
};

let%component make = () => {
  let%hook (stories, setStories) = Hooks.state([]);
  let%hook (offset, setOffset) = Hooks.state(0.);
  let%hook (maybeHeight, setMaybeHeight) = Hooks.state(None);

  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let request = Request.make(HNURLs.topStories);
        let onResponse = r => {
          let body = r |> Result.get_ok |> Response.body;
          switch (Yojson.Safe.from_string(body)) {
          | `List(l) =>
            l
            |> List.map(j =>
                 switch (j) {
                 | `Int(i) => i
                 | _ => failwith("Must be int!")
                 }
               )
            |> (l => setStories(_ => l))
          | _ => failwith("Must be list!")
          };
        };

        ReQuests.perform(~onResponse, request);
        None;
      },
    );

  let onMouseWheel = (event: NodeEvents.mouseWheelEventParams) =>
    setOffset(o =>
      FloatEx.clamp(
        ~min=
          float_of_int((-1) * List.length(stories) * Constants.storyHeight),
        ~max=0.,
        o +. event.deltaY *. Constants.scrollMultiplier,
      )
    );

  let onDimensionsChanged =
      ({height, _}: NodeEvents.DimensionsChangedEventParams.t) =>
    setMaybeHeight(_ => Some(height));

  <View style=Styles.container onMouseWheel onDimensionsChanged>
    <View style={Styles.list(~offset)}>
      {maybeHeight
       |> Option.map(height => {
            stories
            |> List.mapi((idx, id) =>
                 if (idx <= (int_of_float((-1.) *. offset) + height)
                     / Constants.storyHeight) {
                   <Item id />;
                 } else {
                   <View />;
                 }
               )
            |> React.listToElement
          })
       |> Option.value(~default=<View />)}
    </View>
  </View>;
};
