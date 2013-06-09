package
{
	import com.debokeh.anes.utils.USBUtil;
	
	import flash.display.Sprite;
	import flash.text.TextField;
	
	public class Main extends Sprite
	{
		public function Main()
		{
			var result:String = USBUtil.getUSBInfo();
			trace(result);
			
			var tf:TextField;
			TextField(addChild(tf = new TextField)).text = result;
			tf.autoSize = "left";
		}
	}
}