import ReactColorPicker, { useColor } from "react-color-palette";
import classes from "./ColorPicker.module.css";
import "./react-color-palette.css";

type Props = {
  rgb: string;
  onRgbChanged: (rgb: string) => void;
  width: number;
  height: number;
};

const ColorPicker = ({ rgb, onRgbChanged, width, height }: Props) => {
  const [color, setColor] = useColor("hex", rgb);

  return (
    <div
      className={classes.container}
      style={{ backgroundColor: rgb, width, height }}
    >
      <ReactColorPicker
        width={width}
        height={height - 44}
        color={color}
        onChange={(newColor) => {
          const { hex } = newColor;
          onRgbChanged(hex);
          setColor(newColor);
        }}
        hideHEX
        hideHSB
        hideRGB
      />
    </div>
  );
};

export default ColorPicker;
