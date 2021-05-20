module Story = {
  [@deriving (yojson, yojson_fields)]
  [@yojson.allow_extra_fields]
  type t = {
    id: int,
    title: string,
    by: string,
    score: int,
    [@yojson.option]
    url: option(string),
    [@default []]
    kids: list(int),
    descendants: int,
  };
};

module Job = {
  [@deriving (yojson, yojson_fields)]
  [@yojson.allow_extra_fields]
  type t = {
    id: int,
    by: string,
    title: string,
    [@yojson.option]
    text: option(string),
  };
};

module Comment = {
  [@deriving (yojson, yojson_fields)]
  [@yojson.allow_extra_fields]
  type t = {
    id: int,
    [@default ""]
    by: string,
    [@default []]
    kids: list(int),
    [@default ""]
    text: string,
  };
};

type item =
  | Story(Story.t)
  | Job(Job.t)
  | Comment(Comment.t);

let item_of_yojson = (json: Yojson.Safe.t) => {
  switch (json) {
  | `Assoc(l) =>
    switch (
      l
      |> List.find(j => {
           let (key, _) = j;
           key == "type";
         })
    ) {
    | (_, `String("job")) => Job(Job.t_of_yojson(json))
    | (_, `String("story")) => Story(Story.t_of_yojson(json))
    | (_, `String("comment")) => Comment(Comment.t_of_yojson(json))
    | _ => failwith("Invalid item!")
    }
  | _ => failwith("Must be assoc!")
  };
};
