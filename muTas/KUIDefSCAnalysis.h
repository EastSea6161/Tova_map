#pragma  once
namespace UISCAnalysis
{
	const CString UIXAML_SC_BACKIMAGE
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

	const CString UIXAML_SC_MODEL 
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
		_T("    <TextBlock Padding='0, 10, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Screen/Cordon Line </TextBlock>")
		_T("    <TextBlock Padding='0, 10, 0, 0' FontWeight='Bold' Foreground='#003366' HorizontalAlignment='Center'> Analysis </TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_MODEL 
		(
		_T("<Border Width='160' Height='142' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='13'>")     
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>±âÁ¾Á¡ ÅëÇà·® º¸Á¤À» À§ÇÑ</TextBlock>")  
		_T("    <TextBlock TextAlignment='Center' Padding='0, 10, 0, 0' FontWeight='Bold' Foreground='#003366'>Æó¼â¼± ºÐ¼® ¸ðÇü</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_INPUT_DEFAULT
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 15, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Input(Tijm)</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_INPUT_DEFAULT
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>ÀÔ·Â ¼³Á¤</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_INPUT_SELECTED 
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 15, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Input(Tijm)</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_INPUT_SELECTED 
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>ÀÔ·Â ¼³Á¤</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_INPUT_FALSE
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='0, 15, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Input(Tijm)</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_INPUT_FALSE
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>ÀÔ·Â ¼³Á¤</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_SELECT_DEFAULT
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 10, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Select</TextBlock>")    
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Screen/Cordon Line</TextBlock>")
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_SELECT_DEFAULT
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>Æó¼â¼± ¼±ÅÃ</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_SELECT_SELECTED 
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 10, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Select</TextBlock>")    
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Screen/Cordon Line</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_SELECT_SELECTED 
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>Æó¼â¼± ¼±ÅÃ</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_SELECT_FALSE
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='0, 10, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Select</TextBlock>")    
		_T("    <TextBlock Padding='0, 0, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Screen/Cordon Line</TextBlock>")  
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_SELECT_FALSE
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>Æó¼â¼± ¼±ÅÃ</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_CRITERIA_DEFAULT
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 15, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Stopping Criteria</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_CRITERIA_DEFAULT
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>¹Ýº¹¼öÇà Á¾·áÁ¶°Ç</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_CRITERIA_SELECTED 
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")              
		_T("    <TextBlock Padding='0, 15, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Stopping Criteria</TextBlock>")   
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_CRITERIA_SELECTED 
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>¹Ýº¹¼öÇà Á¾·áÁ¶°Ç</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_CRITERIA_FALSE
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='0, 15, 0, 0' FontWeight='Bold' Foreground='#663333' HorizontalAlignment='Center'>Stopping Criteria</TextBlock>") 
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_CRITERIA_FALSE
		(
		_T("<Border Width='116' Height='50' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>¹Ýº¹¼öÇà Á¾·áÁ¶°Ç</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_OUTPUT_DEFAULT
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#ddc09e' />")
		_T("    <Border Height='1' Background='#ffffff' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#663333'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_OUTPUT_DEFAULT
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#faf9e1' Offset='0'/>")
		_T("      <GradientStop Color='#dcaa52' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#663333'>Ãâ·Â ¼³Á¤</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_OUTPUT_SELECTED 
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#7c96b5' />")
		_T("    <Border Height='1' Background='#9acae4' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#003366'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_OUTPUT_SELECTED 
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#378BBA' Offset='0'/>")
		_T("      <GradientStop Color='#CCEBFA' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#003366'>Ãâ·Â ¼³Á¤</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);

	const CString UIXAML_SC_OUTPUT_FALSE
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel TextBlock.FontFamily='Tahoma' TextBlock.FontSize='11'>")       
		_T("    <TextBlock Padding='5, 5, 5, 5'> <Image Source='90927'/> </TextBlock>")    
		_T("    <Border Height='1' Background='#a0a5a8' />")
		_T("    <Border Height='1' Background='#a0a5a8' />")
		_T("    <TextBlock Padding='15, 10, 5, 5' FontWeight='Bold' Foreground='#666666'> Output Design</TextBlock>")     
		_T("  </StackPanel>")
		_T("</Border>")
		);
	const CString KR_UIXAML_SC_OUTPUT_FALSE
		(
		_T("<Border Width='116' Height='116' Margin='0,0,0,0' BorderBrush='Red' BorderThickness='0,0,0,0' CornerRadius='2,2,2,2'>")
		_T("  <Border.Background>")
		_T("    <LinearGradientBrush EndPoint='0.504,1.5' StartPoint='0.504,0.03'>")
		_T("      <GradientStop Color='#9da3a7' Offset='0'/>")
		_T("      <GradientStop Color='#fbfdfd' Offset='0.567'/>")
		_T("    </LinearGradientBrush>")
		_T("  </Border.Background>")        
		_T("  <StackPanel HorizontalAlignment='Center' VerticalAlignment='Center' TextBlock.FontFamily='³ª´®°íµñ' TextBlock.FontSize='11'>")       
		_T("    <TextBlock TextAlignment='Center' FontWeight='Bold' Foreground='#666666'>Ãâ·Â ¼³Á¤</TextBlock>")    
		_T("  </StackPanel>")
		_T("</Border>")
		);
}