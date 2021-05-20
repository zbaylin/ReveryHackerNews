open Revery;
open Revery.UI;
open ReQuests;

open Ppx_yojson_conv_lib;

module Styles = {
  let black = Style.[color(Colors.black)];
  let grey = Style.[color(Colors.grey)];
  let headerContainer =
    Style.[padding(6), borderBottom(~color=Colors.lightGrey, ~width=1)];
  let comment =
    Style.[
      borderLeft(~color=HNColors.orange, ~width=4),
      paddingHorizontal(4),
    ];
  let subComment = Style.[paddingLeft(4)];
  let container = (~level) =>
    level == 0
      ? Style.[borderBottom(~color=Colors.lightGrey, ~width=1)] : [];
};

let%component comment = (~id, ~comments, ~level, ()) => {
  let%hook (comment, setComment) = Hooks.state(None);

  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let request = Request.make(HNURLs.item(id));
        let onResponse = response => {
          let comment =
            try(
              response
              |> Result.get_ok
              |> Response.body
              |> Yojson.Safe.from_string
              |> Models.Comment.t_of_yojson
            ) {
            | Yojson_conv.Of_yojson_error(_, t) =>
              failwith(Yojson.Safe.show(t))
            };

          setComment(_ => Some(comment));
        };
        ReQuests.perform(~onResponse, request);
        None;
      },
    );

  switch (comment) {
  | Some(comment) =>
    <View style={Styles.container(~level)}>
      <View style=Styles.comment>
        <Text
          text={comment.by}
          style=Styles.grey
          fontSize=14.
          fontWeight=Font.Weight.Bold
        />
        <Text text={comment.text} style=Styles.black />
      </View>
      <View style=Styles.subComment>
        <comments kids={comment.kids} level={level + 1} />
      </View>
    </View>
  | None => <View />
  };
};

let rec comments = (~kids: list(int), ~level, ()) => {
  kids |> List.map(id => <comment level comments id />) |> React.listToElement;
};

let%component make = (~id, ()) => {
  let%hook (item, setItem) = Hooks.state(None);

  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let request = Request.make(HNURLs.item(id));
        let onResponse = response => {
          let item =
            response
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

  switch (item) {
  | Some(Story(story)) =>
    <View>
      <View style=Styles.headerContainer>
        <Text
          style=Styles.black
          text={story.title}
          fontWeight=Font.Weight.Bold
          fontSize=20.
        />
        <Text style=Styles.grey text={story.by} fontSize=16. />
      </View>
      <View> <comments kids={story.kids} level=0 /> </View>
    </View>
  | _ => <View />
  };
};
