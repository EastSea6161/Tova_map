#pragma  once
namespace UIDevPlan
{
	const CString UIXAML_DEVPLAN_BACKIMAGE
		(
		_T("<Border  Margin='0,0,0,0' BorderBrush='BLACK' BorderThickness='1,1,1,1' CornerRadius='3,3,3,3'>") 
		_T("  <Border.Background>") 
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>") 
		_T("      <GradientStop Color='#527BA8' Offset='0'/>") 
		_T("      <GradientStop Color='#aac9de' Offset='0.567'/>") 
		_T("    </LinearGradientBrush>")  
		_T("  </Border.Background>")  
		_T("</Border>") 		
		);
	const CString UIXAML_DEVPLAN_MODEL
		(
		_T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='13'>")       
		_T("    <TextBlock Padding='5, 10, 5, 10'><Image Source='90915'/></TextBlock>")    
		_T("    <Border Height='1' Background='#ddc09e' />")
		_T("    <Border Height='1' Background='#ffffff' />")
		_T("    <TextBlock Padding='0, 10, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Development </TextBlock>")
		_T("    <TextBlock Padding='0, 10, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Plan </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString UIXAML_DEVPLAN_DEVPLAN_DEFAULT
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Development</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Plan</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_DEVPLAN_SELECTED
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378bba' Offset='0'/>")
		_T("      <GradientStop Color='#ccebfa' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'>Development</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'>Plan</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_DEVPLAN_FALSE
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'>Development</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'>Plan</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString UIXAML_DEVPLAN_GENERATION_DEFAULT
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Trip</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Generation</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_GENERATION_SELECTED
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378bba' Offset='0'/>")
		_T("      <GradientStop Color='#ccebfa' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'>Trip</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'>Generation</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_GENERATION_FALSE
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'>Trip</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'>Generation</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_DISTRIBUTION_DEFAULT
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Trip</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Distribution</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_DISTRIBUTION_SELECTED
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378bba' Offset='0'/>")
		_T("      <GradientStop Color='#ccebfa' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'>Trip</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'>Distribution</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_DISTRIBUTION_FALSE
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'>Trip</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'>Distribution</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_MODESPLIT_DEFAULT
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Mode</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Split</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_MODESPLIT_SELECTED
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378bba' Offset='0'/>")
		_T("      <GradientStop Color='#ccebfa' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'>Mode</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'>Split</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_MODESPLIT_FALSE
		(
		_T("<Border Width='105' Height='33' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 1, 0, 0' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'>Mode</TextBlock>")
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'>Split</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_OUTPUT_DEFAULT
		(
		_T("<Border Width='120' Height='106' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 45, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Output</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_OUTPUT_SELECTED
		(
		_T("<Border Width='120' Height='106' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378bba' Offset='0'/>")
		_T("      <GradientStop Color='#ccebfa' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 45, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'>Output</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_DEVPLAN_OUTPUT_FALSE
		(
		_T("<Border Width='120' Height='106' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 45, 0, 0' FontWeight='Bold' Foreground='#666666' HorizontalAlignment='Center'>Output</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
}