using SW;
using System;
using System.Xml;

namespace Sandbox
{
	internal class TestEntity : Entity
	{
		[SerializeField]
		private float Velocity = 20.0f;

		protected override void OnCreate()
		{
			Console.WriteLine("OnCreate");

			//using (XmlWriter writer = XmlWriter.Create("books.xml")) {
			//	writer.WriteStartElement("book");
			//	writer.WriteElementString("title", "Graphics Programming using GDI+");
			//	writer.WriteElementString("author", "Mahesh Chand");
			//	writer.WriteElementString("publisher", "Addison-Wesley");
			//	writer.WriteElementString("price", "64.95");
			//	writer.WriteEndElement();
			//	writer.Flush();
			//}
		}

		protected override void OnUpdate(float ts)
		{
			//Velocity += 1.0f;
		}

		protected override void OnLateUpdate(float ts)
		{

		}

		protected override void OnDestroy()
		{
			Console.WriteLine("OnDestroy");
			Console.WriteLine(Velocity);
		}
	}
}
