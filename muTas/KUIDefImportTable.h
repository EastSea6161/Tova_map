#pragma once
namespace UIImportTable
{
	const CString
		UIXAML_I_BACKIMAGE
		(
		_T("<Border  Margin='0,0,0,0' BorderBrush='BLACK' BorderThickness='1,1,1,1' CornerRadius='3,3,3,3'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#ffffff' Offset='0'/>")
		_T("      <GradientStop Color='#ffffff' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")
		_T("</Border>")
		);

	//Node
	const CString 
		UIXAML_I_NODE_FALSE
		(
		_T("<Border Width='105' Height='125' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 50, 5, 5' FontWeight='Bold' Foreground='#666666'> Node Table </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_NODE_DEFAULT
		(
		_T("<Border Width='105' Height='125' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 50, 5, 5' FontWeight='Bold' Foreground='#663333'> Node Table </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_NODE_SELECTED
		(
		_T("<Border Width='105' Height='125' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='15, 50, 5, 5' FontWeight='Bold' Foreground='#003366'> Node Talble </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//LInk
	const CString 
		UIXAML_I_LINK_FALSE
		(
		_T("<Border Width='80' Height='125' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 50, 5, 5' FontWeight='Bold' Foreground='#666666'> Link Table </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_LINK_DEFAULT
		(
		_T("<Border Width='80' Height='125' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 50, 5, 5' FontWeight='Bold' Foreground='#663333'> Link Table </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_LINK_SELECTED
		(
		_T("<Border Width='80' Height='125' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 50, 5, 5' FontWeight='Bold' Foreground='#003366'> Link Talble </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//Zone
	const CString 
		UIXAML_I_ZONE_FALSE
		(
		_T("<Border Width='80' Height='125' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 50, 5, 5' FontWeight='Bold' Foreground='#666666'> Zone Table </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_ZONE_DEFAULT
		(
		_T("<Border Width='80' Height='125' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 50, 5, 5' FontWeight='Bold' Foreground='#663333'> Zone Table </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_ZONE_SELECTED
		(
		_T("<Border Width='80' Height='125' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 50, 5, 5' FontWeight='Bold' Foreground='#003366'> Zone Talble </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//OD
	const CString 
		UIXAML_I_OD_FALSE
		(
		_T("<Border Width='90' Height='60' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='10, 25, 5, 5' FontWeight='Bold' Foreground='#666666'> OD Table </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_OD_DEFAULT
		(
		_T("<Border Width='90' Height='60' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='10, 25, 5, 5' FontWeight='Bold' Foreground='#663333'> OD Table </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_OD_SELECTED
		(
		_T("<Border Width='90' Height='60' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='10, 25, 5, 5' FontWeight='Bold' Foreground='#003366'> OD Talble </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//SED
	const CString 
		UIXAML_I_SED_FALSE
		(
		_T("<Border Width='90' Height='60' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 25, 5, 5' FontWeight='Bold' Foreground='#666666'> SED Table </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_SED_DEFAULT
		(
		_T("<Border Width='90' Height='60' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 25, 5, 5' FontWeight='Bold' Foreground='#663333'> SED Table </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_SED_SELECTED
		(
		_T("<Border Width='90' Height='60' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 25, 5, 5' FontWeight='Bold' Foreground='#003366'> SED Talble </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//Turn
	const CString 
		UIXAML_I_TURN_FALSE
		(
		_T("<Border Width='90' Height='60' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 25, 5, 5' FontWeight='Bold' Foreground='#666666'> Turn Table </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_TURN_DEFAULT
		(
		_T("<Border Width='90' Height='60' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 25, 5, 5' FontWeight='Bold' Foreground='#663333'> Turn Table </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_TURN_SELECTED
		(
		_T("<Border Width='90' Height='60' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 25, 5, 5' FontWeight='Bold' Foreground='#003366'> TURN Talble </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);


	//Transit Info
	const CString 
		UIXAML_I_TRINFO_FALSE
		(
		_T("<Border Width='90' Height='28' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 5, 5, 5' FontWeight='Bold' Foreground='#666666'> Trasit Info </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_TRINFO_DEFAULT
		(
		_T("<Border Width='90' Height='28' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 5, 5, 5' FontWeight='Bold' Foreground='#663333'> Trasit Info </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_TRINFO_SELECTED
		(
		_T("<Border Width='90' Height='28' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 5, 5, 5' FontWeight='Bold' Foreground='#003366'> Trasit Info </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	//Transit Line
	const CString 
		UIXAML_I_TRLINE_FALSE
		(
		_T("<Border Width='90' Height='28' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 5, 5, 5' FontWeight='Bold' Foreground='#666666'> Trasit Line </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_TRLINE_DEFAULT
		(
		_T("<Border Width='90' Height='28' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 5, 5, 5' FontWeight='Bold' Foreground='#663333'> Trasit Line </TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString
		UIXAML_I_TRLINE_SELECTED
		(
		_T("<Border Width='90' Height='28' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='8, 5, 5, 5' FontWeight='Bold' Foreground='#003366'> Trasit Line </TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

}