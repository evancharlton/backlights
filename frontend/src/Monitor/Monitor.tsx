import firebase from "firebase/app";
import "firebase/database";
import { useCallback, useEffect, useMemo, useState } from "react";
import { useThrottleFn } from "react-use";
import ColorPicker from "../ColorPicker";

type Props = {
  dimens: [number, number];
  firebasePath: string;
};

const Monitor = ({ firebasePath, dimens: [width, height] }: Props) => {
  const [rgba, setRgba] = useState("");

  const rgbaRef = useMemo(() => firebase.database().ref(firebasePath), [
    firebasePath,
  ]);

  const onValue = useCallback(
    (data: firebase.database.DataSnapshot) => {
      setRgba(`#${data.val()}`);
    },
    [setRgba]
  );

  useThrottleFn(
    (value) => {
      if (!value) {
        return;
      }

      if (!value.match(/^#[0-9A-Fa-f]{8}$/)) {
        console.warn("Bad format; not writing", value);
        return;
      }

      const cleaned = value.replace("#", "");
      rgbaRef.set(cleaned);
    },
    150,
    [rgba]
  );

  useEffect(() => {
    rgbaRef.on("value", onValue);
    return () => {
      rgbaRef.off("value", onValue);
    };
  }, [onValue, rgbaRef]);

  if (!rgba) {
    return <span>Loading ...</span>;
  }

  let rgb = rgba.substr(0, 7);

  return (
    <ColorPicker
      rgb={rgb}
      onRgbChanged={(newRgb) => setRgba(newRgb + "ff")}
      width={width * 0.15}
      height={height * 0.15}
    />
  );
};

export default Monitor;
