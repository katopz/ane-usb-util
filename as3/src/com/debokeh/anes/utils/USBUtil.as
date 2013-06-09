package com.debokeh.anes.utils
{
	import flash.events.EventDispatcher;
	import flash.external.ExtensionContext;

	/**
	 * Will collect useful USB helper here. 
	 * 
	 * @author katopz
	 * 
	 */
	public class USBUtil extends EventDispatcher
	{
		private static const _EXTENSION_ID:String = "com.debokeh.anes.utils.USBUtil";

		/**
		 * Will get all entries of all USB devices, the USB host controllers, the USB Root hubs , hubs and usb devices.
		 */
		public static function getUSBInfo():String
		{
			var context:ExtensionContext;
			try
			{
				context = ExtensionContext.createExtensionContext(_EXTENSION_ID, 'getUSBInfo');
				return context.call('getUSBInfo') as String;
			}
			catch (error:Error)
			{
				trace(error.message);
			}

			if (context)
			{
				context.dispose();
				context = null;
			}
			
			return null;
		}
	}
}
