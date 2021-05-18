open Revery;
open Revery.UI;
open ReQuests;
open Models.Story;

module Styles = {
  let black = Style.[color(Colors.black)];
  let grey = Style.[color(Colors.grey)];
  let container = (~hovered) =>
    Style.[
      height(Constants.storyHeight),
      padding(6),
      backgroundColor(hovered ? HNColors.selected : HNColors.background),
    ];
  let stats = Style.[alignItems(`FlexEnd)];
  let item =
    Style.[
      flexDirection(`Row),
      justifyContent(`SpaceBetween),
    ];
};

let%component make = (~id: int, ()) => {
  let%hook (item, setItem) = Hooks.state(None);
  let%hook (hovered, setHovered) = Hooks.state(false);

  let onMouseEnter = _ => setHovered(_ => true);
  let onMouseLeave = _ => setHovered(_ => false);
  let onMouseUp = _ => ();

  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let request = Request.make(HNURLs.item(id));
        let onResponse = r => {
          let item =
            r
            |> Result.get_ok
            |> Response.body
            |> Yojson.Safe.from_string
            |> Models.item_of_yojson;

          setItem(_ => Some(item));
        };

        ReQuests.perform(~onResponse, request);
        None;
      },
    );

  <View
    style={Styles.container(~hovered)} onMouseEnter onMouseLeave onMouseUp>
    {switch (item) {
     | None => <Text style=Styles.grey text="Loading..." />
     | Some(Story(story)) =>
       <View style=Styles.item>
         <View>
           {switch (story.url) {
            | Some(url) =>
              <Components.Link
                activeStyle=Styles.black
                inactiveStyle=Styles.black
                text={story.title}
                fontWeight=Font.Weight.Bold
                fontSize=16.
                underlined=false
                href=url
              />
            | None =>
              <Text
                style=Styles.black
                text={story.title}
                fontWeight=Font.Weight.Bold
                fontSize=16.
              />
            }}
           <Text
             style=Styles.grey
             text={story.by}
             fontWeight=Font.Weight.Bold
             fontSize=12.
           />
         </View>
         <View style=Styles.stats>
           <Text
             style=Styles.grey
             text={Printf.sprintf("%d points", story.score)}
             fontSize=13.
           />
           <Text
             style=Styles.grey
             text={Printf.sprintf("%d comments", story.descendants)}
             fontSize=13.
           />
         </View>
       </View>
     | Some(Job(job)) =>
       <View style=Styles.item>
         <View>
           <Text
             style=Styles.black
             text={"[JOB] " ++ job.title}
             fontWeight=Font.Weight.Bold
             fontSize=16.
           />
           <Text
             style=Styles.grey
             text={job.by}
             fontWeight=Font.Weight.Bold
             fontSize=12.
           />
         </View>
       </View>
     }}
  </View>;
};
