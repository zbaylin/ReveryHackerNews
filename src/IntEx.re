let clamp = (~min=Int.min_int, ~max=Int.max_int, i) =>
  if (i < min) {
    min;
  } else if (i > max) {
    max;
  } else {
    i;
  };
