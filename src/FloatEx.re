let clamp = (~min=Float.min_float, ~max=Float.min_float, f) =>
  if (f < min) {
    min;
  } else if (f > max) {
    max;
  } else {
    f;
  };
