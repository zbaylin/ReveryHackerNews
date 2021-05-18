let topStories = "https://hacker-news.firebaseio.com/v0/topstories.json";
let item = (id: int) =>
  "https://hacker-news.firebaseio.com/v0/item/"
  ++ string_of_int(id)
  ++ ".json";
